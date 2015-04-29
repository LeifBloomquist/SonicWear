import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import javax.sound.midi.MidiDevice;
import javax.sound.midi.ShortMessage;


public class Emulator 
{
	static MidiDevice myOutputDevice = null;	
	static int myMidiChannel = 0;

	public static void start(String filename, MidiDevice outputDevice, int channel, int delay_ms) 
	{
		myOutputDevice = outputDevice;
		myMidiChannel = channel;
		
		out("Starting reading file " + filename);
		
		try 
		{			
			List<String> lines = Files.readAllLines(Paths.get(filename));
			
			out("Completed reading file");
			out("Beginning playback:");
			
			for (String l : lines)
			{
				parseAndPlay(l);
				Thread.sleep(delay_ms);
			}
		} 
		catch (Exception e)
		{
			e.printStackTrace();
			System.exit(1);
		}
		
		out("Completed playing file");	
	}

	private static void parseAndPlay(String line) 
	{
		// 1. Parse the Line
		String[] columns = line.split(",");		
		
		if (columns.length != 11) return; 
		
		out(columns[1]); // Time		
		
		int ax = Integer.parseInt(columns[2]);
		int ay = Integer.parseInt(columns[3]);
		int az = Integer.parseInt(columns[4]);
		int gx = Integer.parseInt(columns[5]);
		int gy = Integer.parseInt(columns[6]);
		int gz = Integer.parseInt(columns[7]);
		int mx = Integer.parseInt(columns[8]);
		int my = Integer.parseInt(columns[9]);
		int mz = Integer.parseInt(columns[10]);
		
		// 2. Scaling  (This is taken from the original SoMoV5_Test.ino sketch
		int axcc = Convert.AccelToCC(ax);
		int aycc = Convert.AccelToCC(ay);
		int azcc = Convert.AccelToCC(az);
		int amcc = Convert.AccelMagnitudeToCC(ax,ay,az);
		int gxcc = Convert.GyroToCC(gx);
		int gycc = Convert.GyroToCC(gy);
		int gzcc = Convert.GyroToCC(gz);
		int mxcc = Convert.MagToCC(mx);
		int mycc = Convert.MagToCC(my);
		int mzcc = Convert.MagToCC(mz);
		
		try
		{
			// 3. Create messages
			ShortMessage cc20 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 20, axcc);
			ShortMessage cc21 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 21, aycc);
			ShortMessage cc22 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 22, azcc);
			ShortMessage cc23 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 23, amcc);
			ShortMessage cc24 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 24, gxcc);
			ShortMessage cc25 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 25, gycc);
			ShortMessage cc26 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 26, gzcc);
			ShortMessage cc27 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 27, mxcc);
			ShortMessage cc28 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 28, mycc);
			ShortMessage cc29 = new ShortMessage(ShortMessage.CONTROL_CHANGE, myMidiChannel, 29, mzcc);
			
			// 4. Send
			myOutputDevice.getReceiver().send(cc20, -1);   // -1 for unused Timestamp field
			myOutputDevice.getReceiver().send(cc21, -1);
			myOutputDevice.getReceiver().send(cc22, -1);
			myOutputDevice.getReceiver().send(cc23, -1);
			myOutputDevice.getReceiver().send(cc24, -1);
			myOutputDevice.getReceiver().send(cc25, -1);
			myOutputDevice.getReceiver().send(cc26, -1);
			myOutputDevice.getReceiver().send(cc27, -1);
			myOutputDevice.getReceiver().send(cc28, -1);
			myOutputDevice.getReceiver().send(cc29, -1);
		}		
		catch (Exception e)
		{
			out("Exception during playback: " + e.getMessage());
		}
		
	}

	private static void out(String strMessage)
	{
		System.out.println(strMessage);
	}
}
