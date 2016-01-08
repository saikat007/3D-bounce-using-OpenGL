#include <GL/glut.h>
#include <windows.h>
#include "vec3f.h"

class Ball {
	private:
		//The radius of the ball
		float r;

		//position vector of the ball
        Vec3f pos;

        Vec3f velocity;

		//The angle at which the ball is traveling.  An angle of 0 indicates the
		//positive x direction, while an angle of PI / 2 indicates the positive
		//z direction.  The angle is always between 0 and 2 * PI.
		float angle0;
		//The fraction that the ball is "faded in", from 0 to 1.  It is not 1
		//when the ball is fading in or out.
		float fadeAmount0;
		//Whether the ball is fading out
		bool isFadingOut0;

		//whether the ball collides with object
		bool isCollided;



	public:
		//Constructs a new ball with the specified radius, x,  y and z coordinates,
		//and angle of travel.  An angle of 0 indicates the positive x
		//direction, while an angle of PI / 2 indicates the positive z
		//direction.  The angle must be between 0 and 2 * PI.
		Ball();

		//Returns the radius of the ball
		float radius();
		//Returns the x coordinate of the ball
		float x();

		//returns the z coordinate of the ball
        float y();
		//Returns the z coordinate of the ball
		float z();
		//Returns the angle at which the ball is traveling.  An angle of 0
		//indicates the positive x direction, while an angle of PI / 2 indicates
		//the positive z direction.  The returned angle is between 0 and 2 * PI.
		float angle();

		//Causes the ball to begin fading out
		void fadeOut();
		//Returns whether the ball is fading out
		bool isFadingOut();

		//sets the angle of ball
		void setangle();

		//checks whether the left key is pressed
		void checkleftkey(float);

		//checks whether the right key is pressed
		void checkrightkey(float);

		//checks whether the up key is currently depressed
		void checkupkey(float);

		//checks whether the down key is currently depressed
		void checkdownkey(float);
		void stepup(bool &);

		//checks the collision with walls, if collides returns true
		bool detectCollision(Vec3f);

		//it is called when ball  collides with object
		void CollisionDetected(float , bool&);

};

