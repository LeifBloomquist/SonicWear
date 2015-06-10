
import javax.sound.midi.*;

public class Main 
{

	public static void main(String[] args) throws MidiUnavailableException 
	{
		out("SoMo MIDI Emulator Starting\n");		
	
		String	strFileName = null;
		String	strDeviceName = null;
		String	strMidiChannel = null;
		
		if (args.length == 3)
		{
			strFileName = args[0];
			strDeviceName = args[1];
			strMidiChannel = args[2];
		}
		else
		{
			printUsageAndExit();
		}		
		
		MidiDevice	outputDevice = null;
	
		if (strDeviceName != null)
		{
			MidiDevice.Info	info = MidiCommon.getMidiDeviceInfo(strDeviceName, true);
			if (info == null)
			{
				out("No device info found for name " + strDeviceName);
				System.exit(1);
			}
			
			outputDevice = MidiSystem.getMidiDevice(info);			
			
			if (outputDevice == null)
			{
				out("Wasn't able to retrieve MidiDevice");
				System.exit(1);
			}
			else
			{
				outputDevice.open();
			}
		}
		
		Emulator.start(strFileName, outputDevice, Integer.parseInt(strMidiChannel), 40);
		
		outputDevice.close();
	}
	
	private static void printUsageAndExit()
	{
		out("Usage:");
		out("  java SoMoEmu <filename> <output device> <midi channel>\n");
		MidiCommon.listDevicesAndExit(false, true);
	}

	private static void out(String strMessage)
	{
		System.out.println(strMessage);
	}

}
