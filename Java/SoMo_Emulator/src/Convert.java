
public class Convert 
{
	 // Accelerometer
	
	 private static final int ACCEL_CENTER = 0;         // Assume same for x, y, and z
	 private static final int ACCEL_RANGE  = 16000;
	 private static final int ACCEL_MAX    = (ACCEL_CENTER+ACCEL_RANGE);
	 private static final int ACCEL_MIN    = (ACCEL_CENTER-ACCEL_RANGE);
	 
	 // Gyroscope
	 
	 private static final int GYRO_CENTER  = 0;        // Assume same for x, y, and z
	 private static final int GYRO_RANGE   = 32000;
	 private static final int GYRO_MAX     = (GYRO_CENTER+GYRO_RANGE);
	 private static final int GYRO_MIN     = (GYRO_CENTER-GYRO_RANGE);
	 
	 // Magnetometer	 
	 
	 private static final int MAG_CENTER   = 0;        // Assume same for x, y, and z
	 private static final int MAG_RANGE    = 32000;
	 private static final int MAG_MAX      = (MAG_CENTER+MAG_RANGE);
	 private static final int MAG_MIN      = (MAG_CENTER-MAG_RANGE);
	 
	 // Map an input between MIN and MAX to 0 to 127
	 static int AccelToCC(long in)
	 {   
		 float temp = Scale(in, ACCEL_MIN, ACCEL_MAX);
		 return (int) (temp*127);
	 }
	 
	// Return a value 0 to 127 representing the total magnitude force
	static int AccelMagnitudeToCC(long x, long y, long z)
	{   
	  float tempx = ScaleAbs(x, ACCEL_CENTER, ACCEL_RANGE);
	  float tempy = ScaleAbs(y, ACCEL_CENTER, ACCEL_RANGE);
	  float tempz = ScaleAbs(z, ACCEL_CENTER, ACCEL_RANGE);
	    
	  double magnitudein = Math.sqrt(tempx*tempx + tempy*tempy + tempz+tempz);
	 
	  // Extremely naive, and probably wrong, gravity removal (but it works)
	  double magnitudeout = magnitudein-1.0;
	  
	  // Lower bound here
	  if (magnitudeout < 0.0) magnitudeout = 0.0;
	  
	  // Scale the output to make it less sensitive.  **Adjust this as needed**
	  double magnitudescaled = magnitudeout / 2.5;
	  
	  // Upper Bound
	  if (magnitudescaled > 1.0) magnitudescaled = 1.0;
	  
	  // Determine CC value
	  int cc = (int) (magnitudescaled*127);    
	  
	  return cc;
	}	
	
	// Map an input between MIN and MAX to 0 to 127
	static int GyroToCC(long in)
	{   
	  float temp = Scale(in, GYRO_MIN, GYRO_MAX);
	  return (int) (temp*127);
	}
	
	
	// Map an input between MIN and MAX to 0 to 127
	static int MagToCC(long in)
	{   
	  float temp = Scale(in, MAG_MIN, MAG_MAX);
	  return (int) (temp*127);
	}
	
	
	// Scale one of the inputs to between 0.0 and 1.0
	static float Scale(long in, long smin, long smax)
	{
	  // Bound
	  if (in > smax) in=smax;
	  if (in < smin) in=smin;
	  
	  // Change zero-offset
	  in = in-smin;
	  
	  // Scale between 0.0 and 1.0    (0.5 would be halfway)
	  float temp = (float)in/((float)smax-(float)smin);
	  return temp;   
	}
	
	
	// Scale one of the inputs to return distance from center (0.0=centered, 1.0=min or max, may exceed 1.0)
	static float ScaleAbs(long in, long center, long range)
	{
	  // Get the absolute distance from center
	  long temp = in-center;
	  long val = Math.abs(temp);
	  
	  // Scale
	  float out = (float)val/(float)range;  
	  
	  // *Don't* Bound.
	  
	  return out;
	}
}
