#include "Trackball.h"

Trackball::Trackball() {

	rotMatrix[0] = 1.0f; // Initialize the rotation matrix to identity
	rotMatrix[5] = 1.0f;
	rotMatrix[10] = 1.0f;
	rotMatrix[15] = 1.0f;

	scale = 1.0f;
	this->pressed = false;
}

Trackball::Trackball(double scale) {

	rotMatrix[0] = 1.0f; // Initialize the rotation matrix to identity
	rotMatrix[5] = 1.0f;
	rotMatrix[10] = 1.0f;
	rotMatrix[15] = 1.0f;

	this->scale = scale;
	this->pressed = false;
}

// Indicate that the trackball is being pressed if it is not already, and calculate the initial point
void  Trackball::press(Point point) {
	if (!pressed) {
		previousPoint.x = -point.x; // Remember the point that was pressed
		previousPoint.y = -point.y;
		previousPoint.z = 0.0f;
		if (previousPoint.getMagnitude() > 1.0f)
			previousPoint = previousPoint.normalize();
		double magnitude = (1.0f - pow(point.x,2) - pow(point.y,2));
		if (magnitude <= 0.0f) // Make sure the point is not beyond the trackball
			previousPoint.z = 0.0f;
		else // If the point is on the trackball, calculate the z coordinate
			previousPoint.z = sqrt(magnitude);
		pressed = true;
	}
}

// Indicate that the trackball is no longer being pressed
void Trackball::release() {
	pressed = false;
}

// Calculates the trackball's rotation matrix when the mouse is moved across it
void  Trackball::move(Point point) {
	if (pressed) {
		calculateMatrix(point); // Calculate the new matrix based on the new point the mouse has been dragged to

		previousPoint.x = -point.x; // Calculate the new previous point
		previousPoint.y = -point.y;
		previousPoint.z = 0.0f;
		if (previousPoint.getMagnitude() > 1.0f)
			previousPoint = previousPoint.normalize();
		double magnitude = (1.0f - pow(point.x,2) - pow(point.y,2));
		if (magnitude <= 0.0f)
			previousPoint.z = 0.0f;
		else
			previousPoint.z = sqrt(magnitude);
	}
}

// Resets the trackball's rotation matrix to identity
void Trackball::reset() {
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15)
			rotMatrix[i] = 1.0f;
		else
			rotMatrix[i] = 0.0f;
	}
}

// Returns the rotation matrix of the trackball
float* Trackball::getMatrix() {
	return rotMatrix;
}

// Returns the current point of the trackball
Vector3 Trackball::getPoint() {
	return spherePoint;
}

// Calculate the trackball's rotation matrix from its current state
void Trackball::calculateMatrix(Point point) {

	spherePoint.x = -point.x; // Calculate the current point
	spherePoint.y = -point.y;
	double magnitude = (1.0f - pow(point.x,2) - pow(point.y,2));
	if (magnitude <= 0.0f)
		spherePoint.z = 0.0f;
	else
		spherePoint.z = sqrt(magnitude);

	if (spherePoint.getMagnitude() > 1.0f)
		return;	//Don't allow invalid coordinates for the sphere point

	Vector3 axis = Vector3::cross(spherePoint, previousPoint); // Get the axis of rotation from the two point vectors
	float theta = asin(axis.getMagnitude())*scale; // Get the angle of the rotation from the magnitude of the axis and the defined trackball scale

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // save the old matrix so we don't mess anything up
	glLoadIdentity();
	glRotatef(theta, axis.x, axis.y, axis.z); // our newly calculated rotation
	glMultMatrixf(rotMatrix); // our previous rotation matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*) rotMatrix);
	glPopMatrix(); // return modelview to its old value;
}
