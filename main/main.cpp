#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

#include "defaults.hpp"
#include "loadobj.hpp"
#include "cylinder.hpp"
#include "cone.hpp"
#include "cube.hpp"


namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - CW2";

// ***************************************************************
	// Global definition of pi
	constexpr float kPi_ = 3.1415926f;
	// Speed of the camera
	float kMovementPerSecond_ = 5.f; // units per second
	// Mouse sensitivity
	constexpr float kMouseSensitivity_ = 0.01f; // radians per pixel

	// Struct to manage different states the applications will be in
	struct State_
	{
		// Define the shader programs
		ShaderProgram* prog;
		ShaderProgram* progMat;
		// Boolean to trigger the animation
		bool animationActive;
		// Boolean to trigger splitscreen
		bool splitActive;
		// Start time for animation
		Clock::time_point animationStartTime;
		int currentCam;

		// Define the attributes for the camera
		struct CamCtrl_
		{
			// This state tells us to modify the camera if activated
			bool cameraActive;
	
			// Define bool values
			// These values will be toggled when the relevant key is pressed
			bool actionMoveForward, actionMoveBack;
			bool actionMoveRight, actionMoveLeft;
			bool actionMoveUp, actionMoveDown;
			
			// Toggles for the speed
			bool speed, slow;
			
			// Values for the camera
			float phi, theta;
			float radius;

			// Ever since the day i was born i have been tracking my last x and y values
			float lastX, lastY;
			Vec3f cameraPos;
			
		} camControl;
	};

	
	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );

	void glfw_callback_motion_( GLFWwindow*, double, double );


	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
}

int main() try
{
	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Configure GLFW and create window
	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		1280,
		720,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	// Set up event handling


	State_ state{};
	glfwSetWindowUserPointer( window, &state );
	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetCursorPosCallback( window, &glfw_callback_motion_ );


	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Initialize GLAD
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
#	if !defined(NDEBUG)
	setup_gl_debug_output();
#	endif // ~ !NDEBUG

	// Global GL state
	OGL_CHECKPOINT_ALWAYS();

	glEnable( GL_FRAMEBUFFER_SRGB );
	glEnable( GL_CULL_FACE );
	glClearColor( 0.2f, 0.2f, 0.2f, 0.0f );
	glEnable( GL_DEPTH_TEST );


	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

// ***************************************************************
	// Load shader programs
	ShaderProgram prog( {
		{ GL_VERTEX_SHADER, "assets/default.vert" },
		{ GL_FRAGMENT_SHADER, "assets/default.frag" }
	} );

	ShaderProgram progMat( {
		{ GL_VERTEX_SHADER, "assets/mat.vert" },
		{ GL_FRAGMENT_SHADER, "assets/mat.frag" }
	} );

	// Define the shader programs
	state.prog = &prog;
	state.progMat = &progMat;

	// Animation state
	auto last = Clock::now();


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Other initialization & loading
	OGL_CHECKPOINT_ALWAYS();
	

// ***************************************************************
	auto land = load_wavefront_obj("assets/parlahti.obj");
	auto pad = load_wavefront_obj("assets/landingpad.obj");
	GLuint tex = load_texture_2d("assets/L4343A-4k.jpeg");

// SPACESHIP CODE
// ----------------------------------------------------------------
	// Make the objects for the ship and concatenate them
	auto mainBody = make_cylinder( true, 16, {0.9098f, 0.5137f, 0.0627f},
		make_scaling( 0.7f, 0.1f, 0.1f )
	);
	auto topCone = make_cone( true, 16, {1.f, 1.f, 1.f},
		make_scaling( 0.13f, 0.1f, 0.1f ) * make_translation( { 5.385f, 0.f, 0.f } )
	);
	auto topConnect = concatenate( std::move(mainBody), topCone );

	auto baseCube = make_cube( {0.9098f, 0.5137f, 0.0627f},
		make_scaling( 0.24f, 0.2f, 0.2f ) * make_translation( { -0.5f, 0.f, 0.f } )
	);
	auto bottomConnect = concatenate( std::move(topConnect), baseCube );

	auto mainThruster = make_cone( true, 16, {0.01f, 0.01f, 0.01f},
		make_scaling( 0.2f, 0.1f, 0.1f ) * make_translation( { -1.6f, 0.f, 0.f } )
	);
	auto mainRocket = concatenate( std::move(bottomConnect), mainThruster );

	auto sideBody1 = make_cylinder( true, 16, {1.f, 1.f, 1.f},
		make_scaling( 0.7f, 0.08f, 0.08f ) * make_translation( { -0.4f, 2.25f, 0.f } )
	);
	auto sideRocketStep1 = concatenate( std::move(mainRocket), sideBody1 );

	auto sideCone1 = make_cone( true, 16, {0.01f, 0.01f, 0.01f},
		make_scaling( 0.08f, 0.08f, 0.08f ) * make_translation( { 5.25f, 2.25f, 0.f } )
	);
	auto sideRocketStep2 = concatenate( std::move(sideRocketStep1), sideCone1 );

	auto sideThruster1 = make_cone( true, 16, {0.01f, 0.01f, 0.01f},
		make_scaling( 0.2f, 0.08f, 0.08f ) * make_translation( { -1.8f, 2.25f, 0.f } )
	);
	auto sideRocketStep3 = concatenate( std::move(sideRocketStep2), sideThruster1 );

	auto sideBody2 = make_cylinder( true, 16, {1.f, 1.f, 1.f},
		make_scaling( 0.7f, 0.08f, 0.08f ) * make_translation( { -0.4f, -2.25f, 0.f } )
	);
	auto sideRocketStep4 = concatenate( std::move(sideRocketStep3), sideBody2 );

	auto sideCone2 = make_cone( true, 16, {0.01f, 0.01f, 0.01f},
		make_scaling( 0.08f, 0.08f, 0.08f ) * make_translation( { 5.25f, -2.25f, 0.f } )
	);
	auto sideRocketStep5 = concatenate( std::move(sideRocketStep4), sideCone2 );

	auto sideThruster2 = make_cone( true, 16, {0.01f, 0.01f, 0.01f},
		make_scaling( 0.2f, 0.08f, 0.08f ) * make_translation( { -1.8f, -2.25f, 0.f } )
	);
	auto ship = concatenate( std::move(sideRocketStep5), sideThruster2 );

	 

	// Create the vaos for the objects
	GLuint vao = create_vao( land );
	GLuint vaoPad =  create_vao( pad );
	GLuint vaoShip = create_vao( ship );

	// Assign vertex counts
	std::size_t vertexCount = land.positions.size();
	std::size_t vertexCountPad = pad.positions.size();
	std::size_t vertexCountShip = ship.positions.size();

	OGL_CHECKPOINT_ALWAYS();

	// Initialising OpenGL queries
	GLuint frameTime, task2Time, task4Time, task5Time;
	glGenQueries(1, &frameTime);
	glGenQueries(1, &task2Time);
	glGenQueries(1, &task4Time);
	glGenQueries(1, &task5Time);

	// Initialising timestamps
	GLuint64 frameTimeT, task2TimeT, task4TimeT, task5TimeT;

	// Variable for tracking clock time / ticks
	auto frameToFramePrev = std::chrono::high_resolution_clock::now();

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Begin query to track frame render time
		glBeginQuery(GL_TIME_ELAPSED, frameTime);

		// Let GLFW process events
		glfwPollEvents();
		
		// Check if window was resized.
		float fbwidth, fbheight;
		int nwidth, nheight;
		glfwGetFramebufferSize( window, &nwidth, &nheight );

		fbwidth = float(nwidth);
		fbheight = float(nheight);

		if( 0 == nwidth || 0 == nheight )
		{
			// Window minimized? Pause until it is unminimized.
			// This is a bit of a hack.
			do
			{
				glfwWaitEvents();
				glfwGetFramebufferSize( window, &nwidth, &nheight );
			} while( 0 == nwidth || 0 == nheight );
		}

		if (state.splitActive) {
			glViewport( 0, 0, nwidth/2, nheight );
		}
		else {
			glViewport( 0, 0, nwidth, nheight );
		}

		// Begin query to track task 2 render time
		glBeginQuery(GL_TIME_ELAPSED, task2Time);

		// Update state

// ***************************************************************
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
		last = now;


		// // Update camera state
		if( state.camControl.actionMoveForward )
		{
			// this vector points in the direction the camera is facing
			Vec3f directionVector = normalize(Vec3f
			{
				std::sin(state.camControl.phi),
			 	std::sin(state.camControl.theta),
			 	-(std::cos(state.camControl.phi))
			 });
			
			// the normalised vector can be used to update the position vector
			state.camControl.cameraPos.x += kMovementPerSecond_ * dt * directionVector.x;
			state.camControl.cameraPos.y -= kMovementPerSecond_ * dt * directionVector.y;
			state.camControl.cameraPos.z += kMovementPerSecond_ * dt * directionVector.z;

		}
			
		if( state.camControl.actionMoveBack )
		{
			// this vector points in the direction the camera is facing
			Vec3f directionVector = normalize(Vec3f
			{
				std::sin(state.camControl.phi),
			 	std::sin(state.camControl.theta),
			 	-(std::cos(state.camControl.phi))
			 });

			// the normalised vector can be used to update the position vector
			state.camControl.cameraPos.x -= kMovementPerSecond_ * dt * directionVector.x;
			state.camControl.cameraPos.y += kMovementPerSecond_ * dt * directionVector.y;
			state.camControl.cameraPos.z -= kMovementPerSecond_ * dt * directionVector.z;

		}

		
		if( state.camControl.actionMoveLeft )
		{	

			// initialise the up vector of the camera this will be constant
			Vec3f upVector = {0.0f, 1.0f, 0.0f};
			// initialise the direction vector like before
			Vec3f directionVector = normalize(Vec3f
			{
				std::sin(state.camControl.phi),
			 	std::sin(state.camControl.theta),
			 	-(std::cos(state.camControl.phi))
			 });

			// define the the right vector which is the cross product of the up vector and Direction vector
			Vec3f rightVec = cross(upVector, directionVector);

			// update the position vector to move left
			state.camControl.cameraPos.x += kMovementPerSecond_ * dt * rightVec.x;
			state.camControl.cameraPos.y += kMovementPerSecond_ * dt * rightVec.y;
			state.camControl.cameraPos.z += kMovementPerSecond_ * dt * rightVec.z;
		}


		if( state.camControl.actionMoveRight )
		{
			// initialise the up vector of the camera this will be constant
			Vec3f upVector = {0.0f, 1.0f, 0.0f};
			// initialise the direction vector like before
			Vec3f directionVector = normalize(Vec3f
			{
				std::sin(state.camControl.phi),
			 	std::sin(state.camControl.theta),
			 	-(std::cos(state.camControl.phi))
			 });

			// define the the right vector which is the cross product of the up vector and Direction vector
			Vec3f rightVec = cross(upVector, directionVector);

			// update the position vector to move right
			state.camControl.cameraPos.x -= kMovementPerSecond_ * dt * rightVec.x;
			state.camControl.cameraPos.y -= kMovementPerSecond_ * dt * rightVec.y;
			state.camControl.cameraPos.z -= kMovementPerSecond_ * dt * rightVec.z;
		}

		// just update the y value of the position vector to move up
		if( state.camControl.actionMoveUp )
			state.camControl.cameraPos.y -= kMovementPerSecond_ * dt;

		// just update the y value of the position vector to move down
		if( state.camControl.actionMoveDown )
			state.camControl.cameraPos.y += kMovementPerSecond_ * dt;

		kMovementPerSecond_ = 5.f;
		// Adjust speed based on what key is pressed
		if(state.camControl.speed)
		{
			kMovementPerSecond_ = kMovementPerSecond_ * 4;
		}

		if(state.camControl.slow)
		{
			kMovementPerSecond_ = kMovementPerSecond_ * 0.5;
		}

		// Model to world for the spaceship
		Mat44f model2world4 ;

		if (state.animationActive)
		{
			// Start animaiton time
			auto const animationTime = std::chrono::duration_cast<Secondsf>(Clock::now() - state.animationStartTime).count();

			// Calculate position along the curved path (you can adjust the formula for a different path)
			float animationSpeed = 0.05 * animationTime;
			float animationDistance = animationSpeed * animationTime;
			
			// Creating rotation and z increase
			float rotationIncrease = 5.f * std::atan(0.0125f*(animationTime-5));
			float zIncrease = 90.f * std::atanh(0.004f*(animationTime-5));

			// Updating positions and rotaions for the first 5 seconds
			if(animationTime < 5) {
				model2world4 =  make_translation( { -5.7f, -0.6f + animationDistance, -2.f} );
				model2world4 = model2world4 * make_rotation_z(1.5708f);
				// Fixed following camera
				if((state.currentCam == 1)) {
					state.camControl.cameraPos.x = -11.7f;
					state.camControl.cameraPos.y = -0.6f + animationDistance;
					state.camControl.cameraPos.z = -8.f;
					state.camControl.phi = kPi_*0.75;
					state.camControl.theta = 0.f;
				}
				// Ground camera
				if((state.currentCam == 2)) {
					state.camControl.cameraPos.x = 4.3f;
					state.camControl.cameraPos.y = -0.6f;
					state.camControl.cameraPos.z = -2.f;
					state.camControl.phi = kPi_*1.5;
					state.camControl.theta = -(std::atan(animationDistance/10));
			}
			}
			// Positions for the next 5 seconds
			else if(animationTime > 5 && animationTime < 10) {
				model2world4 =  make_translation( { -5.7f, -0.6f + animationDistance, -2.f- zIncrease} );
				model2world4 = model2world4 * make_rotation_z(1.5708f);
				model2world4 = model2world4 * make_rotation_y(rotationIncrease);
				// Fixed camera
				if((state.currentCam == 1)) {
					state.camControl.cameraPos.x = -11.7f;
					state.camControl.cameraPos.y = -0.6f + animationDistance;
					state.camControl.cameraPos.z = -8.f - zIncrease;
					state.camControl.phi = kPi_*0.75;
					state.camControl.theta = -rotationIncrease/kPi_;
				}
				// Ground camera
				if((state.currentCam == 2)) {
					state.camControl.cameraPos.x = 4.3f;
					state.camControl.cameraPos.y = -0.6f;
					state.camControl.cameraPos.z = -2.f;
					state.camControl.phi = kPi_*1.5 + std::atan(zIncrease/10);
					state.camControl.theta = -(std::atan(animationDistance/10));
				}
			}
			else{
				// Rest of the animation
				rotationIncrease = 5.f * std::atan(0.0125f*(5));
				model2world4 =  make_translation( { -5.7f, -0.6f + animationDistance, -2.f - zIncrease} );
				model2world4 = model2world4 * make_rotation_z(1.5708f);
				model2world4 = model2world4 * make_rotation_y(rotationIncrease);
				// FIxed camera
				if((state.currentCam == 1)) {
					state.camControl.cameraPos.x = -11.7f;
					state.camControl.cameraPos.y = -0.6f + animationDistance;
					state.camControl.cameraPos.z = -8.f - zIncrease;
					state.camControl.phi = kPi_*0.75;
					state.camControl.theta = -rotationIncrease/kPi_;
				}
				// Ground camera
				if((state.currentCam == 2)) {
					state.camControl.cameraPos.x = 4.3f;
					state.camControl.cameraPos.y = -0.6f;
					state.camControl.cameraPos.z = -2.f;
					state.camControl.phi = kPi_*1.5 + std::atan(zIncrease/10);
					state.camControl.theta = -(std::atan(animationDistance/10));
				}
			}

			// Adjust animation time
			if (animationTime > 30.0f) 
			{
				state.animationActive = false;
			}
		}
		else {
			// Reset ship positions
			model2world4 =  make_translation( { -5.7f, -0.6f, -2.f } );
			model2world4 = model2world4 * make_rotation_z(1.5708f);
		}

		// Model for the map
		Mat44f model2world =  make_translation( { 0.f, 0.f, 0.f } );
		Mat33f normalMatrix = mat44_to_mat33( transpose(invert(model2world)) );
		
		// Components for the cameras translation matrix
		Mat44f Rx = make_rotation_x( state.camControl.theta );
		Mat44f Ry = make_rotation_y( state.camControl.phi );
		Mat44f T = make_translation(-state.camControl.cameraPos);

		// Translatuion matrix for the xamera
		Mat44f world2camera = Rx * Ry * T;

		// Projection
		Mat44f projection = make_perspective_projection(
			60.f * 3.1415926f / 180.f,
			fbwidth/float(fbheight),
			0.1f, 100.0f
		);

		// ProjcameraWorld to be added to the uniform matrix
		Mat44f projCameraWorld = projection * world2camera * model2world;

		// End query to track task 2 render time
		glEndQuery(GL_TIME_ELAPSED);

		// Getting query result and casting to float
		glGetQueryObjectui64v(task2Time, GL_QUERY_RESULT, &task2TimeT);
		float task2TimeFloat = static_cast<float>(task2TimeT);

		// Print time to render frame in terminal
		std::printf("\n--------------------------------------------------------------\n\n");
		std::printf("Frame - Task 2 Rendering Time: %.9f ms\n", task2TimeFloat * 1e-6);

		// Begin query to track task 4 render time
		glBeginQuery(GL_TIME_ELAPSED, task4Time);

		// Add the first launchpad to the world
		Mat44f model2world2 =  make_translation( { -24.5f, -0.97f, -54.f } );
		Mat33f normalMatrix2 = mat44_to_mat33( transpose(invert(model2world2)) );
		Mat44f projCameraWorld2 = projection * world2camera * model2world2;

		// Add the second launchpad to the world
		Mat44f model2world3 =  make_translation( { -5.7f, -0.97f, -2.f } );
		Mat33f normalMatrix3 = mat44_to_mat33( transpose(invert(model2world3)) );
		Mat44f projCameraWorld3 = projection * world2camera * model2world3;

		// End query to track task 4 render time
		glEndQuery(GL_TIME_ELAPSED);

		// Getting query result and casting to float
		glGetQueryObjectui64v(task4Time, GL_QUERY_RESULT, &task4TimeT);
		float task4TimeFloat = static_cast<float>(task4TimeT);
		// Print time to render frame in terminal
		std::printf("Frame - Task 4 Rendering Time: %.9f ms\n", task4TimeFloat * 1e-6);

		// Begin query to track task 5 render time
		glBeginQuery(GL_TIME_ELAPSED, task5Time);

		// nNOrmal matrix to be passed into shader
		Mat33f normalMatrix4 = mat44_to_mat33( transpose(invert(model2world4)) );
		Mat44f projCameraWorld4 = projection * world2camera * model2world4;

		// End query to track task 5 render time
		glEndQuery(GL_TIME_ELAPSED);

		glGetQueryObjectui64v(task5Time, GL_QUERY_RESULT, &task5TimeT);
		float task5TimeFloat = static_cast<float>(task5TimeT);
		// Print time to render frame in terminal
		std::printf("Frame - Task 5 Rendering Time: %.9f ms\n", task5TimeFloat * 1e-6);
		
		
		OGL_CHECKPOINT_DEBUG();

		// Defining variable for CPU clock
		auto renderCommandsStart = std::chrono::high_resolution_clock::now();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Using default frag/vert
		glUseProgram(prog.programId());
		
		// Main world
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld.v
		);

		// Applying textures
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, tex );

		// Normals for main world
		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix.v
		);

		// Bind vao and draw
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
		glBindVertexArray(0);

		// Set uniform values for lighting in the fragment shader
		Vec3f lightDir = normalize(Vec3f{ 0.f, 1.f, -1.f });
		glUniform3fv(2, 1, &lightDir.x);
		glUniform3f(3, 0.9f, 0.9f, 0.6f);
		glUniform3f(4, 0.1f, 0.1f, 0.1f);

		// Define positions for point lights and pass into default shader
		Vec3f pointLightPos{ model2world4(0,3), model2world4(1,3), model2world4(2,3)};  // Replace with the actual position of your point light 
		Vec3f pointLightViewPos1{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
		glUniform3fv(5, 1, &pointLightPos.x);
		glUniform3fv(6, 1, &pointLightViewPos1.x);


		Vec3f pointLightPos2{model2world4(0,3), model2world4(1,3)+2.25f, model2world4(2,3)};
		Vec3f pointLightViewPos2{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
		glUniform3fv(7, 1, &pointLightPos2.x);
		glUniform3fv(8, 1, &pointLightViewPos2.x);

		Vec3f pointLightPos3{model2world4(0,3), model2world4(1,3)-2.25f, model2world4(2,3)};
		Vec3f pointLightViewPos3{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
		glUniform3fv(9, 1, &pointLightPos3.x);
		glUniform3fv(10, 1, &pointLightViewPos3.x);


		// Using mat frag/vert
		glUseProgram(progMat.programId());


		// Landing pad 1 
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld2.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix2.v
		);

		glUniformMatrix4fv(
			11,
			1, GL_TRUE, model2world2.v
		);


		// Bind vao and draw
		glBindVertexArray(vaoPad);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountPad));
		glBindVertexArray(0);


		// Landing pad 2
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld3.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix3.v
		);

		glUniformMatrix4fv(
			11,
			1, GL_TRUE, model2world3.v
		);

		// Bind vao and draw
		glBindVertexArray(vaoPad);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountPad));
		glBindVertexArray(0);


		// Ship
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld4.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix4.v
		);

		glUniformMatrix4fv(
			11,
			1, GL_TRUE, model2world4.v
		);

		// Bind vao and draw
		glBindVertexArray(vaoShip);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountShip));
		glBindVertexArray(0);
		
		// Set uniform values for lighting in the fragment shader
		Vec3f lightDir2 = normalize(Vec3f{ 0.f, 1.f, -1.f });
		glUniform3fv(2, 1, &lightDir2.x);

		// Directional light for materials
		glUniform3f(3, 0.9f, 0.9f, 0.6f);
		glUniform3f(4, 0.05f, 0.05f, 0.05f); 


		// Reference positions for point lights and pass into mat shader
		glUniform3fv(5, 1, &pointLightPos.x);
		glUniform3fv(6, 1, &pointLightViewPos1.x);

		glUniform3fv(7, 1, &pointLightPos2.x);
		glUniform3fv(8, 1, &pointLightViewPos2.x);

		glUniform3fv(9, 1, &pointLightPos3.x);
		glUniform3fv(10, 1, &pointLightViewPos3.x);


		// If split screen is active
		if (state.splitActive) {
			// Change viewport
			glViewport( nwidth/2, 0, nwidth/2, nheight );
			
			// Draw everything again in that viewport

			// Using default frag/vert
			glUseProgram(prog.programId());
			
			// Main world
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld.v
			);

			// Applying textures
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, tex );

			// Normals for main world
			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix.v
			);

			// Bind vao and draw
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
			glBindVertexArray(0);

			// Set uniform values for lighting in the fragment shader
			Vec3f lightDir = normalize(Vec3f{ 0.f, 1.f, -1.f });
			glUniform3fv(2, 1, &lightDir.x);
			glUniform3f(3, 0.9f, 0.9f, 0.6f);
			glUniform3f(4, 0.1f, 0.1f, 0.1f);

			// Define positions for point lights and pass into default shader
			Vec3f pointLightPos{ model2world4(0,3), model2world4(1,3), model2world4(2,3)};  // Replace with the actual position of your point light 
			Vec3f pointLightViewPos1{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
			glUniform3fv(5, 1, &pointLightPos.x);
			glUniform3fv(6, 1, &pointLightViewPos1.x);


			Vec3f pointLightPos2{model2world4(0,3), model2world4(1,3)+2.25f, model2world4(2,3)};
			Vec3f pointLightViewPos2{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
			glUniform3fv(7, 1, &pointLightPos2.x);
			glUniform3fv(8, 1, &pointLightViewPos2.x);

			Vec3f pointLightPos3{model2world4(0,3), model2world4(1,3)-2.25f, model2world4(2,3)};
			Vec3f pointLightViewPos3{model2world4(0,3), model2world4(1,3), model2world4(2,3)};
			glUniform3fv(9, 1, &pointLightPos3.x);
			glUniform3fv(10, 1, &pointLightViewPos3.x);


			// Using mat frag/vert
			glUseProgram(progMat.programId());


			// Landing pad 1 
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld2.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix2.v
			);

			glUniformMatrix4fv(
				11,
				1, GL_TRUE, model2world2.v
			);


			// Bind vao and draw
			glBindVertexArray(vaoPad);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountPad));
			glBindVertexArray(0);


			// Landing pad 2
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld3.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix3.v
			);

			glUniformMatrix4fv(
				11,
				1, GL_TRUE, model2world3.v
			);

			// Bind vao and draw
			glBindVertexArray(vaoPad);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountPad));
			glBindVertexArray(0);


			// Ship
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld4.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix4.v
			);

			glUniformMatrix4fv(
				11,
				1, GL_TRUE, model2world4.v
			);

			// Bind vao and draw
			glBindVertexArray(vaoShip);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCountShip));
			glBindVertexArray(0);
			
			// Set uniform values for lighting in the fragment shader
			Vec3f lightDir2 = normalize(Vec3f{ 0.f, 1.f, -1.f });
			glUniform3fv(2, 1, &lightDir2.x);

			// Directional light for materials
			glUniform3f(3, 0.9f, 0.9f, 0.6f);
			glUniform3f(4, 0.05f, 0.05f, 0.05f); 


			// Reference positions for point lights and pass into mat shader
			glUniform3fv(5, 1, &pointLightPos.x);
			glUniform3fv(6, 1, &pointLightViewPos1.x);

			glUniform3fv(7, 1, &pointLightPos2.x);
			glUniform3fv(8, 1, &pointLightViewPos2.x);

			glUniform3fv(9, 1, &pointLightPos3.x);
			glUniform3fv(10, 1, &pointLightViewPos3.x);
		}

		OGL_CHECKPOINT_DEBUG();

		// End query to track frame render time
		glEndQuery(GL_TIME_ELAPSED);

		// Getting query result and casting to float
		glGetQueryObjectui64v(frameTime, GL_QUERY_RESULT, &frameTimeT);
		float frameTimeFloat = static_cast<float>(frameTimeT);
		// Print time to render frame in terminal
		std::printf("Frame - Full Rendering Time: %.9f ms\n", frameTimeFloat * 1e-6);

		// Logic to get cpu tick rate, convert to ms and print to term
		auto frameToFrameEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> frameToFrameTime = frameToFrameEnd - frameToFramePrev;
		auto frameToFrameTimeMilli = std::chrono::duration_cast<std::chrono::milliseconds>(frameToFrameTime);
		float frameToFrameTimeFloat = frameToFrameTimeMilli.count();
		std::printf("\n");
		std::printf("Frame-To-Frame Time: %.6f ms\n", frameToFrameTimeFloat);

		// Resetting start time for frame-to-frame performance measure
		frameToFramePrev = std::chrono::high_resolution_clock::now();

		// Display results
		glfwSwapBuffers( window );

		// Logic to get cpu tick rate, convert to ms and print to term
		auto renderCommandsEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> renderCommandsTime = renderCommandsEnd - renderCommandsStart;
		auto renderCommandsTimeMilli = std::chrono::duration_cast<std::chrono::milliseconds>(renderCommandsTime);
		float renderCommandsTimeFloat = renderCommandsTimeMilli.count();
		std::printf("Submitting rendering commands time: %.6f ms\n", renderCommandsTimeFloat);
	}

	// Cleanup
	state.prog = nullptr;
	state.progMat = nullptr;
	
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

// ***************************************************************
	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		// Escape closes window
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}

		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			// R-key reloads shaders.
			if( GLFW_KEY_R == aKey && GLFW_PRESS == aAction )
			{
				if( state->prog )
				{
					try
					{
						state->prog->reload();
						std::fprintf( stderr, "Shaders reloaded and recompiled.\n" );
					}
					catch( std::exception const& eErr )
					{
						std::fprintf( stderr, "Error when reloading shader:\n" );
						std::fprintf( stderr, "%s\n", eErr.what() );
						std::fprintf( stderr, "Keeping old shader.\n" );
					}
				}
			}

			// Space toggles camera
			if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
			{
				state->camControl.cameraActive = !state->camControl.cameraActive;

				if( state->camControl.cameraActive )
					glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
				else
					glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
			}

			// Camera controls if camera is active
			if( state->camControl.cameraActive )
			{
				if( state->camControl.cameraActive )
				{
					// Key presses resulting in movement
					if( GLFW_KEY_W == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveForward = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveForward = false;
					}
					else if( GLFW_KEY_S == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveBack = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveBack = false;
					}

					if( GLFW_KEY_A == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveLeft = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveLeft = false;
					}
					else if( GLFW_KEY_D == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveRight = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveRight = false;
					}

					if( GLFW_KEY_Q == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveUp = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveUp = false;
					}

					else if( GLFW_KEY_E == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.actionMoveDown = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.actionMoveDown = false;
					}
					
					// Key presses resulting in speed chnages
					if( GLFW_KEY_LEFT_SHIFT == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.speed = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.speed = false;
					}

					
					if( GLFW_KEY_LEFT_CONTROL == aKey )
					{
						if( GLFW_PRESS == aAction )
							state->camControl.slow = true;
						else if( GLFW_RELEASE == aAction )
							state->camControl.slow = false;
					}

					// Key press resulting in animation
					if (GLFW_KEY_F == aKey && GLFW_PRESS == aAction)
					{
						state->animationActive = true;
						state->animationStartTime = Clock::now();
						state->currentCam = 0;
					}

					// Key press reseting animation
					else if (GLFW_KEY_R == aKey && GLFW_PRESS == aAction)
					{
						state->animationActive = false;
					}
					// Key press resulting in split screen
					if (GLFW_KEY_V == aKey && GLFW_PRESS == aAction)
					{
						state->splitActive = !state->splitActive;
					}
				
				}
				if (state->animationActive) {
					// Key press to toggle through camera modes
					if (GLFW_KEY_C == aKey && GLFW_PRESS == aAction)
					{
						state->currentCam = (state->currentCam + 1) % 3;
					}
				}

			}
		}
	}

	void glfw_callback_motion_( GLFWwindow* aWindow, double aX, double aY )
	{
		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			if( state->camControl.cameraActive )
			{
				auto const dx = float(aX-state->camControl.lastX);
				auto const dy = float(aY-state->camControl.lastY);

				state->camControl.phi += dx*kMouseSensitivity_;
				
				state->camControl.theta += dy*kMouseSensitivity_;
				if( state->camControl.theta > kPi_/2.f )
					state->camControl.theta = kPi_/2.f;
				else if( state->camControl.theta < -kPi_/2.f )
					state->camControl.theta = -kPi_/2.f;
			}

			state->camControl.lastX = float(aX);
			state->camControl.lastY = float(aY);
		}
	}
}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}

