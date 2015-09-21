using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SomoTestApp
{
    public partial class MainForm : Form
    {
        public const int NumDevices=4;

        private SOMO[] SoMoDevices =  new SOMO[NumDevices];
        private List<byte> DataStream = new List<byte>();
        private SerialPort serialport = null;

        // Animation Timer
        private System.Windows.Forms.Timer AnimTimer = new System.Windows.Forms.Timer();
        private double[] unitpulse = new double[NumDevices];
        private double messagepulse = 0.0; 

        public MainForm()
        {
            InitializeComponent();

            COMPorts.SelectedItem = "COM Port";
            BaudRate.SelectedItem = "38400";

            for (int i = 0; i < SoMoDevices.Length; i++)
            {
                SoMoDevices[i] = new SOMO();
                SoMoDevices[i].stopwatch = new System.Diagnostics.Stopwatch();
            }

            // Start the animation timer.
            AnimTimer.Tick += TimerEventProcessor;
            AnimTimer.Interval = 50;    // Sets the timer interval to 50 milliseconds.
            AnimTimer.Start();
        }

        protected override CreateParams CreateParams
        {
            get
            {
                const int WS_EX_COMPOSITED = 0x02000000;
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= WS_EX_COMPOSITED;
                return cp;
            }
        } 

        private void COMPorts_Click(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();

            COMPorts.Items.Clear();
            COMPorts.Items.AddRange(ports);
            COMPorts.SelectedIndex = 0;
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            // Disconnect in case already connected.
            if (serialport != null)
            {
                serialport.Close();
            }
            
            // Connect the COM port.
            serialport = new SerialPort(COMPorts.Text, Int32.Parse(BaudRate.Text), Parity.None, 8, StopBits.One);                             

            try
            {
                serialport.Open();
            }
            catch (Exception ex)
            {
                Error("Exception: " + ex.Message.ToString());
                return;
            }

            if (!serialport.IsOpen)  // Failed to open COM port
            {
                Error("Failed to open COM port");
                return;
            }

            serialport.DataReceived += serialport_DataReceived;

        }

        void serialport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // Get the data
            SerialPort sp = (SerialPort)sender;
            byte[] indata = new byte[sp.BytesToRead];
            sp.Read(indata, 0, sp.BytesToRead);
            ParseData(indata);
        }

        void ParseData(byte[] incoming)
        {
            DataStream.AddRange(incoming);

           

            // Look for a FF in the message.  Indicates end of "packet".
            int eom = DataStream.IndexOf(255);
            if (eom == -1)
            {
                // Prevent unending messages
                if (DataStream.Count > 1000)
                {
                    byte[] message1 = DataStream.ToArray();
                    DataStream.Clear();
                    Receive(message1);
                }
                return;  // No match yet
            }
           
            var tempmessage = DataStream.Take(eom+1);   // 0-indexed
            byte[] message = tempmessage.ToArray();
            DataStream.RemoveRange(0, eom+1);

            Receive((byte[])message);
        }

        void Receive(byte[] message)
        {
            BeginInvoke((Action)(() =>
            {
                RawDataStreamText.AppendText(Environment.NewLine + ByteArrayToString(message));
            }), null);

            // Sanity Checks
            if (message[0] != 0x8F)
            {
                Error("Invalid header byte: " + message[0]);
                return;  
            }

            // Sanity Checks
            if (message.Length != 29)
            {
                Error("Invalid message length: " + message.Length);
                return;
            }

            int unitid = -1000;

            try
            {
                unitid = message[7] - 0x31;

                if ((unitid > 3) || (unitid < 0))
                {
                    Error("Unknown Unit ID: " + unitid + " (" + message[7] + ")");
                    return;
                }

                SoMoDevices[unitid].UnitID = unitid;

                // Record timestamps etc.
                SoMoDevices[unitid].NumReceived++;
                HandleTiming(SoMoDevices[unitid]);               

                // Parse the rest of the data
                SoMoDevices[unitid].AX = (message[9] << 7) + message[8];
                SoMoDevices[unitid].AY = (message[11] << 7) + message[10];
                SoMoDevices[unitid].AZ = (message[13] << 7) + message[12];
                SoMoDevices[unitid].MX = (message[15] << 7) + message[14];
                SoMoDevices[unitid].MY = (message[17] << 7) + message[16];
                SoMoDevices[unitid].MZ = (message[19] << 7) + message[18];
                SoMoDevices[unitid].RX = (message[21] << 7) + message[20];
                SoMoDevices[unitid].RY = (message[23] << 7) + message[22];
                SoMoDevices[unitid].RZ = (message[25] << 7) + message[24];
            }
            catch (Exception ex)
            {
                Error("Exception: " + ex.Message.ToString());
            }

            Display(SoMoDevices[unitid]);
        }

        void HandleTiming(SOMO somo)
        {
            somo.LastDelta = somo.stopwatch.Elapsed;
            somo.stopwatch.Restart();

            float average = somo.Average.Ticks;
            float x = somo.LastDelta.Ticks;            
            float count = somo.NumReceived;

            average = (x-average ) / count;

            somo.Average += TimeSpan.FromTicks((long)average);
        }

        void Display(SOMO somo)
        {
            int offset = 0;

            if (ApplyOffsetCheckBox.Checked)
            {
                offset = -512;
            }

            // A bit kludgy - find the GUI controls to update

            TextBox tb = null;

            switch (somo.UnitID)
            {
                case 0: tb = InfoBox1;
                    break;

                case 1: tb = InfoBox2;
                    break;

                case 2: tb = InfoBox3;
                    break;

                case 3: tb = InfoBox4;
                    break;

                default:
                    Error("Invalid Unit ID: " + somo.UnitID);
                    return;
            }

            BeginInvoke((Action)(() =>
            {
                tb.Text =
                 "Last Delta: " + (somo.LastDelta.Seconds * 1000 + somo.LastDelta.Milliseconds) + " ms" + Environment.NewLine +
                 "Ave. Delta: " + (somo.Average.Seconds * 1000 + somo.Average.Milliseconds) + " ms" + Environment.NewLine +
                 "# Messages: " + somo.NumReceived + Environment.NewLine + Environment.NewLine +
                  
                 "AX: " + (somo.AX + offset) + Environment.NewLine +
                 "AY: " + (somo.AY + offset) + Environment.NewLine +
                 "AY: " + (somo.AZ + offset) + Environment.NewLine +
                 "RX: " + (somo.RX + offset) + Environment.NewLine +
                 "RY: " + (somo.RY + offset) + Environment.NewLine +
                 "RZ: " + (somo.RZ + offset) + Environment.NewLine +
                 "MX: " + (somo.MX + offset) + Environment.NewLine +
                 "MY: " + (somo.MY + offset) + Environment.NewLine +
                 "MZ: " + (somo.MZ + offset) + Environment.NewLine;
               
            }), null);

            unitpulse[somo.UnitID] = 1.0;
        }

        private void ClearStatsButton_Click(object sender, EventArgs e)
        {
            foreach (SOMO s in SoMoDevices)
            {
                s.Average = TimeSpan.Zero;
                s.LastDelta = TimeSpan.Zero;
                s.NumReceived = 0;
            }

            ErrorMessages.Text = "";
            RawDataStreamText.Text = "";
        }

        private void DisconnectButton_Click(object sender, EventArgs e)
        {
            if (serialport != null)
            {
                serialport.Close();
            }
        }

        public static string ByteArrayToString(byte[] ba)
        {
            string hex = BitConverter.ToString(ba);
            return hex.Replace("-", " ");
        }

        void Error(String errormessage)
        {
            BeginInvoke((Action)(() =>
            {
                ErrorMessages.AppendText(Environment.NewLine + DateTime.Now.ToString("yyyy-MM-dd - HH:mm:ss.fff") + " :: " + errormessage);
            }), null);
            
            messagepulse = 1.0;
        }

        // This is the method to run when the timer is raised.
        // Animates the background of the TX and RX LEDs.
        private void TimerEventProcessor(Object myObject, EventArgs myEventArgs)
        {
            // Unit Colors #1
            int green = (int)(unitpulse[0] * 128);
            InfoBox1.BackColor = Color.FromArgb(255 - green, 255, 255 - green);

            // Unit Colors #2
            green = (int)(unitpulse[1] * 128);
            InfoBox2.BackColor = Color.FromArgb(255 - green, 255, 255 - green);

            // Unit Colors #3
            green = (int)(unitpulse[2] * 128);
            InfoBox3.BackColor = Color.FromArgb(255 - green, 255, 255 - green);

            // Unit Colors #4
            green = (int)(unitpulse[3] * 128);
            InfoBox4.BackColor = Color.FromArgb(255 - green, 255, 255 - green);

            // Messages Colors
            int red = (int)(messagepulse * 255);
            ErrorMessages.BackColor = Color.FromArgb(255, 255-red,255-red);

            // Decay Effect
            for (int i = 0; i < unitpulse.Length; i++)
            {
                unitpulse[i] *= 0.5;
            }

            messagepulse *= 0.7;
        }

        private void FakeDataButton_Click(object sender, EventArgs e)
        {
            //byte[] fake = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 255, 13 };
            //byte[] fake = { 0x8F, 1, 2, 3, 4, 5, 6, 0x33, 8, 9, 10, 11, 12, 255 };
            byte[] fake = { 0x8F, 1, 2, 3, 4, 5, 6, 0x33, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255 };


            ParseData(fake);
        }
    }
}
