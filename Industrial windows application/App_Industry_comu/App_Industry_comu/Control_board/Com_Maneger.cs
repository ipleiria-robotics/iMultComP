using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;
using System.Windows.Forms;
using System.Drawing; 


namespace App_Industry_comu.Control_board
{

    class Com_Maneger
    {
        #region "Maneger Enums"
        public enum TransmissionType
        {
            TEXT, 
            HEX
        }
        

        public enum RxMode
        {
            ASCII, 
            DEC, 
            HEX, 
            BIN
        }
        

        public enum MessageType
        {
            INCOMING, 
            OUTGOING, 
            NORMAL,
            WARNING,
            ERROR
        }
        #endregion

        #region "Variables_ENUMS"
            private string _baudRate = string.Empty;
            private string _parity = string.Empty;
            private string _stopBits = string.Empty;
            private string _dataBits = string.Empty;
            private string _portName = string.Empty; 
            private TransmissionType  _transType; 
            private RxMode rx_Mode;
            private RichTextBox _displayWindow;
            private ProgressBar progress_Bar;
            private TextBox text_information;
            private Label label;
            private string _msg = string.Empty;
            private MessageType message_type;
            private Color[] MessageColor =
            {
                 Color.Blue,
                 Color.Green,
                 Color.Black, 
                 Color.Orange,
                 Color.Red,
            };
            private SerialPort ComPort = new SerialPort();
        #endregion

        private bool write = true; 

        #region "Maneger Properties"  
            public string Baudrate
            {
                get { return _baudRate; }
                set { _baudRate = value;  }
            }
            public string Parity
            {
                get { return _parity;  }
                set { _parity = value;  }
            }
            public string StopBits
            {
                get { return _stopBits;  }
                set { _stopBits = value;  }
            }
            public string DataBits
            {
                get { return _dataBits;  }
                set { _dataBits = value;  }
            }
            public string PortName
            {
                get { return _portName;  }
                set { _portName = value;  }
            }
            public TransmissionType CurrentTransmissionType
            {
                get { return _transType;  }
                set { _transType = value;  }
            }
            public RichTextBox DisplayWindow
            {
                get { return _displayWindow;  }
                set { _displayWindow = value;  }
            }
            public RxMode Current_rxMode
            {
                get { return rx_Mode; }
                set { rx_Mode = value; }
            }      
            public TextBox display_text_information
            {
                get { return text_information; }
                set { text_information = value;  }
            }
            //  Property to hold the message being sent
            // through the serial port
        
            public string Message
            {
                get { return _msg;  }
                set { _msg = value;  }
            }

            public MessageType Type
            {
                get { return message_type;  }
                set { message_type = value;  }
            }

            public ProgressBar Signal_progress_bar
            {
                get { return progress_Bar;  }
                set { progress_Bar = value;  }
            }
        #endregion

        public Label signal_label
        {
            get { return label;  }
            set { label = value;  }
        }

        #region "Manager Constructors"
            public Com_Maneger(string baud, string par, string stop_Bits, string data_bits, string name,  RichTextBox rbt)
            {
                _baudRate = baud;
                _parity = par;
                _stopBits = stop_Bits;
                _portName = name;
                _displayWindow = rbt;
                ComPort.DataReceived += comPort_DataReceived; 
            }

            // Constructor to set the properties of our
            // serial port communicator to nothing
            public Com_Maneger()
            {
                _baudRate = string.Empty;
                _parity = string.Empty;
                _stopBits = string.Empty;
                _dataBits = string.Empty;
                _portName = "COM1";
                _displayWindow = null;
                ComPort.DataReceived += comPort_DataReceived; 
            }

            public Com_Maneger(ref ProgressBar progress_bar)
            {
                _baudRate = string.Empty;
                _parity = string.Empty;
                _stopBits = string.Empty;
                _dataBits = string.Empty;
                _portName = "COM1";
                _displayWindow = null;
                ComPort.DataReceived += comPort_DataReceived;
            }
        #endregion

        #region "Write_Data"
             public void Write_Data(string msg)
            {
                try
                {
                   switch (CurrentTransmissionType)
                   {
                    case TransmissionType.TEXT:
                        if(!(ComPort.IsOpen == true))
                        {
                            ComPort.Open(); 
                        }

                        // send the message to the port 
                        ComPort.Write(msg + Environment.NewLine);

                        // display the message
                        message_type = MessageType.OUTGOING;
                        // + " " + /n + " "
                        _msg = msg;
                        DisplayData(message_type, _msg); 
                        break;
                    case TransmissionType.HEX:

                        try
                        {
                            // convert the message to byte array 
                            byte[] newMsg = HexToByte(msg);
                            if (!write)
                            {
                                DisplayData(message_type, _msg);
                                return; 
                            }
                            // send the message to the port 
                            ComPort.Write(newMsg, 0, newMsg.Length);
                            // convert back to hex and display 
                            message_type = MessageType.OUTGOING;

                            _msg = ByteToHex(newMsg);
                            DisplayData(message_type, _msg); 

                        }catch(FormatException ex)
                        {
                            // display error message 
                            message_type = MessageType.ERROR;
                            _msg = ex.Message + "" + Environment.NewLine + " ";
                            DisplayData(message_type, _msg);
                        }
                        finally
                        {
                            _displayWindow.SelectAll(); 
                        }

                        break;

                    default:
                        // first make sure the port in open 
                        // if its not open then open it 
                        if(!(ComPort.IsOpen == true))
                        {
                            ComPort.Open(); 
                        }

                        // send the message to the port 
                        ComPort.Write(msg);

                        // display the message 
                        message_type = MessageType.OUTGOING;
                        _msg = msg;
                        DisplayData(MessageType.OUTGOING, _msg); 
                        break; 
                   } 
                }
                catch(Exception ex)
                {

                }
            }
        #endregion


        #region "comPort_DataReceived"
           private void comPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
           {
            /// <summary>
            /// method that will be called when theres data waiting in the buffer
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            //determine the mode the user selected (binary/string)
            switch (CurrentTransmissionType)
                {
          
                case TransmissionType.TEXT:
                    // user chose string 
                    // read data and waiting in the buffer
                     string msg_recived = ComPort.ReadExisting();

                    //MessageBox.Show(msg)
                    try
                    {

                    }catch ( Exception Ex)
                    {

                    }

                    // display the data to the User 
                    message_type = MessageType.INCOMING;
                    _msg = msg_recived;
                    DisplayData(MessageType.INCOMING, msg_recived + " " + Environment.NewLine + " "); 
                    break;

                case TransmissionType.HEX:
                    //user chose binary
                    //retrieve number of bytes in the buffer
                    int bytes = ComPort.BytesToRead;
                    // create a bytes array to hold the awaiting data 
                    byte[] comBuffer = new byte[bytes];
                    // read the data and store it 
                    ComPort.Read(comBuffer, 0, bytes);
                    // display the data to the user 
                    message_type = MessageType.INCOMING;
                    _msg = ByteToHex(comBuffer) + "" + Environment.NewLine + "";
                    DisplayData(MessageType.INCOMING, ByteToHex(comBuffer) + "" + Environment.NewLine + ""); 

                    break;

                default:
                    // read data waiting in the buffer 
                    string str = ComPort.ReadExisting();
                    try
                    {
                                                
                    }catch(Exception ex)
                    {

                    }
                    // display the data to the user 
                    message_type = MessageType.INCOMING;
                    _msg = str + "" + Environment.NewLine + "";
                    DisplayData(MessageType.INCOMING, str + "" + Environment.NewLine + ""); 
                    break;                
                }
           }
        #endregion


        #region "SetParatyValues"
        public void SetParityValues(object obj)
        {
            foreach(string str in Enum.GetNames(typeof(Parity)))
            {
                ((ComboBox)obj).Items.Add(str); 
            }
        }
        #endregion

        #region "SetStopBitValues"
        public void SetStopBitValues(object obj)
        {
            foreach(string str in Enum.GetNames(typeof(StopBits)))
            {
                ((ComboBox)obj).Items.Add(str); 
            }
        }
        #endregion

        #region "SetPortNameValues"
        public void SetPortNameValues(object obj)
        {
            foreach(string str in SerialPort.GetPortNames())
            {
                ((ComboBox)obj).Items.Add(str); 
            }
        }
        #endregion

        #region "OpenPort"
        public bool Open_Port()
        {
            try
            {
                // frist check if port is already open, if yes close it 
                if(ComPort.IsOpen == true)
                {
                    ComPort.Close(); 
                }

                // set the properties of our SerialPort Object 
                ComPort.BaudRate = int.Parse(_baudRate);
                // BaudRate 
                ComPort.DataBits = int.Parse(_dataBits);
                // stopbits
                ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), _stopBits);
                // parity 
                ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), _parity);
                // portName 
                ComPort.PortName = _portName;
                // open port 
                ComPort.Open();

                // display msg 
                message_type = MessageType.NORMAL;
                _msg = "Port opened \n";
                DisplayData(message_type, _msg); 
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message); 
                DisplayData(message_type, _msg); 
                return false; 
            }

            
        }
        #endregion

        #region "ClosePort"
        public void ClosePort()
        {
            if(ComPort.IsOpen)
            {
                _msg = "Port Close \n";
                message_type = MessageType.NORMAL;
                DisplayData(message_type, _msg);
                ComPort.Close(); 
            }
        }
        #endregion

        #region "DisplayData"
        /// Method to display the data to and
        /// from the port on the screen    
        [STAThread()]  // single thread 
        private void DisplayData(MessageType messagetype, string msg)
        {
            try
            {
                _displayWindow.Invoke(new EventHandler(DoDisplay));
            }
            catch(Exception ex)
            {
                
            }
             
        }
        #endregion

        #region "DO_Display"
        //  private RichTextBox _displayWindow; 
        private void DoDisplay(object sender, EventArgs e)
            {
                _displayWindow.SelectedText = string.Empty;
                _displayWindow.SelectionFont = new Font(_displayWindow.SelectionFont, FontStyle.Bold);
                _displayWindow.SelectionColor = MessageColor[Convert.ToInt32(message_type)];
                _displayWindow.AppendText(_msg + " ");
                _displayWindow.ScrollToCaret(); 
            }
        #endregion

        #region "BytesToHex"
            /// <summary>
            ///  Method to convert a bhte array into a hex string 
            /// </summary>
            /// <param name="combytes"> byte array to convert </param>
            /// <returns> a hex string </returns>
        private string ByteToHex(byte[] comByte)
        {
            // create a new strinfBuilder object 
            StringBuilder builder = new StringBuilder(comByte.Length * 3); 
            // loop througt each byte in the array 
            foreach (byte data in comByte)
            {
                builder.Append(Convert.ToString(data, 16).PadLeft(2, '0').PadRight(3, ' '));
                //convert the byte to a string and add to the stringbuilder
            }

            // return the convertes value 
            return builder.ToString().ToUpper(); 
        }
        #endregion


        #region "HexToByte"
        /// <summary>
        /// method to convert hex string into a byte array 
        /// </summary>
        /// <param name="msg"></param>
        /// <returns> a byte array  </returns>
        private byte[] HexToByte(string msg)
        {
            if(msg.Length % 2 ==0)
            {
                // remove any spaces from the string 
                _msg = msg;
                _msg = msg.Replace(" ", "");
                // create a byte array  -> length divided 2 ( hex is 2 characters in lenght)
                byte[] comBuffer = new byte[_msg.Length / 2]; 

                for(int i=0; i <= _msg.Length -1; i += 2)
                {
                    comBuffer[i / 2] = Convert.ToByte(Convert.ToByte(_msg.Substring(i, 2), 16)); 
                }
                write = true;
                return comBuffer; 
            }
            else
            {
                _msg = "Invalid format";
                message_type = MessageType.ERROR;
                // 
                write = false;
                return null; 
            }

        }

        #endregion

        public string Remove(string value, string rmv)
        {
            int pos = value.IndexOf(rmv); 
            if(pos >= 0)
            {
                return value.Remove(pos, rmv.Length); 
            }
            return value; 
        }
    }
}
