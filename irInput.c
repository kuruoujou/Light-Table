/* This is just a function, we can combine it later. */
/* It also needs to be modified to work with the micro, this is
* just a generic code listing for now. */

/* Requires a pin number, returns a duty cycle (0 - 100) */
/* Assumes a lot of things - mostly globals. */

int IRreceive(int pin){
	//Perform ATD conversion on pin
	//This is almost certainly wrong, needs updated.
	int duty;
	atdctl5 = pin;
	while (atdstat == 0) {
		continue;
	}
	//output pwm duty cycle
}
