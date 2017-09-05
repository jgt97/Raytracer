MODULES:
	Parser:
		implementation and header file that parses the JSON file into a scene using Qt
	Scene
		header file defining structs to contain the information describing the scene from the Parser module
	RayTrace
		the actual ray tracing computation is done here. Uses Scene to describe the scene and Ray to represent rays in the scene
	Ray
		module implementing class Ray to provide convenient operations on vectors, such as dot product
	threadsafe_queue
		module implementing the thread safe queue from the example in class