using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports; 


namespace App_Industry_comu
{
    public partial class modbus_win : Form
    {
        // osbject private EasyModbus, 
        private EasyModbus.ModbusClient modbusClient;



        public modbus_win()
        {
            InitializeComponent();
            modbusClient = new EasyModbus.ModbusClient();
            // modbusClient.ReceiveDataChanged += new EasyModbus.ModbusClient.ReceiveDataChangedHandler();
            modbusClient.SendDataChanged += new EasyModbus.ModbusClient.SendDataChangedHandler(Update_send_Data);



            comboBox_Serial_port_SelectedIndexChanged(null, null);
            Verificar_btn_color_connect(null, null); 
        }

        // --------------------------------------------------------------------------------------------------------
        string sendData = null; 
        void Update_send_Data(object sender)
        {
            sendData = "Tx: " + BitConverter.ToString(modbusClient.sendData).Replace("-", " ") + System.Environment.NewLine;
            Thread thread = new Thread(updateSend_textBox);
            thread.Start();
        }

        delegate void UpdateReceivedDataCallback(); 
        void updateSend_textBox()
        {
           if(textBox_information.InvokeRequired)
            {
                UpdateReceivedDataCallback d = new UpdateReceivedDataCallback(updateSend_textBox);
                this.Invoke(d, new object[] { });
            }
            else
            {
                textBox_information.AppendText(sendData);
            }
        }

        //---------------------------------------------------------------------------------------

        private void Combox_type_comunication_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (modbusClient.Connected) modbusClient.Disconnect(); 

            if(Combox_type_comunication.SelectedIndex == 0)   // if modbus rtpc 
            {

                label_sever_ip.Visible = true; 
                label_Server_port.Visible = true;
                textBox_IP.Visible = true;
                textBox_port.Visible = true;

                // invisibility property modbus RTU 
                label_COM_port.Visible = false;
                label_slave_ID.Visible = false;
                label_baudrate.Visible = false;
                label_Parity.Visible = false;
                label_stop_bit.Visible = false;
                comboBox_Serial_port.Visible = false;
                comboBox_parity.Visible = false;
                comboBox_stopbits.Visible = false; 
                textBox_Baud.Visible = false; 


            }
            if(Combox_type_comunication.SelectedIndex == 1)   // modbus select RTU 
            {
                // esconde as definioes do TCP IP Modbus 
                label_Server_port.Visible = false;
                label_sever_ip.Visible = false;
                textBox_IP.Visible = false;
                textBox_port.Visible = false;


                label_COM_port.Visible = true;
                label_slave_ID.Visible = true;
                label_baudrate.Visible = true;
                label_Parity.Visible = true;
                label_stop_bit.Visible = true;
                comboBox_Serial_port.Visible = true;
                comboBox_parity.Visible = true;
                comboBox_stopbits.Visible = true;
                textBox_Baud.Visible = true;
              //  comboBox_Serial_port.SelectedIndex = 0;     //select serial port 
                comboBox_parity.SelectedIndex = 0;           // select parity 
                //comboBox_parity.SelectedIndex = 0;           // select parity 
                comboBox_stopbits.SelectedIndex = 0; 

              //  if (comboBox_Serial_port.SelectedText == "") comboBox_Serial_port.SelectedItem.ToString(); 


            } 

        }
          // 
        private void comboBox_Serial_port_SelectedIndexChanged(object sender, EventArgs e)
        {
            String[] ArrayComPortsAvailable = null;
            int index = -1;
            String ComPortName = null;

            if (modbusClient.Connected) modbusClient.Disconnect();
            //modbusClient.SerialPort = comboBox_Serial_port.SelectedItem.ToString();
            //modbusClient.UnitIdentifier = byte.Parse(textBox_Slave_ID.Text); 

            ArrayComPortsAvailable = SerialPort.GetPortNames();
            try
            {
                do
                {
                    index += 1;
                    comboBox_Serial_port.Items.Add(ArrayComPortsAvailable[index]);
                } while (!((ArrayComPortsAvailable[index] == ComPortName) || (index == ArrayComPortsAvailable.GetUpperBound(0))));

                Array.Sort(ArrayComPortsAvailable);

                if (index == ArrayComPortsAvailable.GetUpperBound(0))
                {
                    ComPortName = ArrayComPortsAvailable[0];
                }

                if (comboBox_Serial_port.Items[0] == null)
                {
                    comboBox_Serial_port.Items[0] = "COM 1";
                }
            }
            catch (Exception ex)
            {

            }


            

        }

        private void Btn_connect_Click(object sender, EventArgs e)
        {
            try
            {
                // disconnect, caso necessario 
                if (modbusClient.Connected) modbusClient.Disconnect(); 

                // selecionado o modo modus TCP                  
                if(Combox_type_comunication.SelectedIndex == 0)
                {
                    modbusClient.IPAddress = textBox_IP.Text;
                    modbusClient.Port = int.Parse(textBox_port.Text);
                    modbusClient.SerialPort = null;

                    modbusClient.Connect();
                }
                if(Combox_type_comunication.SelectedIndex ==1)
                {
                    // information Serial port modbus 
                    modbusClient.SerialPort = comboBox_Serial_port.SelectedItem.ToString();
                    modbusClient.UnitIdentifier = byte.Parse(textBox_Slave_ID.Text);
                    modbusClient.Baudrate = int.Parse(textBox_Baud.Text);
                    if (comboBox_parity.SelectedIndex == 0) modbusClient.Parity = System.IO.Ports.Parity.Even;
                    if (comboBox_parity.SelectedIndex == 1) modbusClient.Parity = System.IO.Ports.Parity.Even;
                    if (comboBox_parity.SelectedIndex == 2) modbusClient.Parity = System.IO.Ports.Parity.None;

                    if (comboBox_stopbits.SelectedIndex == 0) modbusClient.StopBits = System.IO.Ports.StopBits.One;
                    if (comboBox_stopbits.SelectedIndex == 1) modbusClient.StopBits = System.IO.Ports.StopBits.OnePointFive;
                    if (comboBox_stopbits.SelectedIndex == 2) modbusClient.StopBits = System.IO.Ports.StopBits.Two;

                    modbusClient.Connect(); 

                    

                }


            }
            catch (Exception excession)
            {
                MessageBox.Show(excession.Message, "Error to connect server", MessageBoxButtons.OK, MessageBoxIcon.Error); 
            }

            Verificar_btn_color_connect(null, null);
        }

        private void Btn_disconnect_Click(object sender, EventArgs e)
        {
            modbusClient.Disconnect();
            Verificar_btn_color_connect(null, null);
        }

        private void textBox_IP_TextChanged(object sender, EventArgs e)
        {

        }

       
        private void textBox_Baud_TextChanged(object sender, EventArgs e)
        {
            if (modbusClient.Connected) modbusClient.Disconnect();
            modbusClient.Baudrate = int.Parse(textBox_Baud.Text); 
        }

        
        // funçao para mostrar a indicaçao de connecçao 
        private void Verificar_btn_color_connect(object sender, EventArgs e)
        {
            if (modbusClient.Connected)
            {
                Btn_connect.BackColor = Color.Green; 
            }
            else
            {
                Btn_connect.BackColor = Color.Red;
            }
        }

        // btn para preparar registros ou coils 
        bool listBox_prepere_COils = false;
        bool listBox_prepere_Register = false; 
        private void Btn_prepere_coils_Click(object sender, EventArgs e)
        {
            if (!listBox_prepere_COils)
            {
                // apagar a lisbox de ler os registros do modbus 
            }
            listBox_prepere_COils = true;
            listBox_prepere_Register = false;
            listBox_Write_values.Items.Add(textBox_prepere_coils.Text); 

        }
        private void Btn_prepere_registers_Click(object sender, EventArgs e)
        {
            if (!listBox_prepere_Register)
            {
                // apagar a lisbox de ler os registros do modbus 
            }
            listBox_prepere_Register = true;
            listBox_prepere_COils = false;
            listBox_Write_values.Items.Add(int.Parse(textBox_prepere_register.Text)); 
        }

        // troca de TRUE or FALSE 
        private void textBox_prepere_coils_DoubleClick(object sender, EventArgs e)
        {
            if (textBox_prepere_coils.Text.Equals("FALSE")) textBox_prepere_coils.Text = "TRUE";
            else textBox_prepere_coils.Text = "FALSE"; 
        }

        private void Btn_write_single_coils_Click(object sender, EventArgs e)
        {
            try
            {
                // if server to disconnect, connect now again 
                if (!modbusClient.Connected)
                {
                    Btn_connect_Click(null, null); 
                }
                bool coils_to_send = false;
                coils_to_send = bool.Parse(listBox_Write_values.Items[0].ToString());
                modbusClient.WriteSingleCoil(int.Parse(textBox_write_address.Text) - 1, coils_to_send);   // starting address , value 
            }
            catch(Exception exc)
            {
                MessageBox.Show(exc.Message, "Exception Write values to server ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Btn_write_single_registers_Click(object sender, EventArgs e)
        {
            try
            {
                if (!modbusClient.Connected)
                {
                    Btn_connect_Click(null, null); 
                }
                int register_to_send = 0;
                register_to_send = int.Parse(listBox_Write_values.Items[0].ToString());
                modbusClient.WriteSingleRegister(int.Parse(textBox_write_address.Text) - 1, register_to_send); 

            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Exception Write values to server ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Bnt_write_multi_coils_Click(object sender, EventArgs e)
        {
            try
            {
                if (!modbusClient.Connected)
                {
                    Btn_connect_Click(null, null); 
                }

                bool[] coils_array_to_send = new bool[listBox_Write_values.Items.Count]; 
                for(int i=0; i< listBox_Write_values.Items.Count; i++)
                {
                    coils_array_to_send[i] = bool.Parse(listBox_Write_values.Items[i].ToString()); 
                }
                modbusClient.WriteMultipleCoils(int.Parse(textBox_write_address.Text) - 1, coils_array_to_send);
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Exception Write values to server ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        private void Btn_write_milti_register_Click(object sender, EventArgs e)
        {
            try
            {
                if (!modbusClient.Connected)
                {
                    Btn_connect_Click(null, null);
                }

                int[] register_array_to_send = new int[listBox_Write_values.Items.Count]; 

                for(int i=0; i<listBox_Write_values.Items.Count; i++)
                {
                    register_array_to_send[i] = int.Parse(listBox_Write_values.Items[i].ToString());
                }
                modbusClient.WriteMultipleRegisters(int.Parse(textBox_write_address.Text) - 1, register_array_to_send);

            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Exception Write values to server ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void modbus_win_Load(object sender, EventArgs e)
        {

        }

        private void textBox_port_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox_Slave_ID_TextChanged(object sender, EventArgs e)
        {
            try
            {
                modbusClient.UnitIdentifier = byte.Parse(textBox_Slave_ID.Text);
            }catch(FormatException )
            {

            }
        }

        private void comboBox_parity_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_stopbits_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void listBox_Write_values_MouseDoubleClick(object sender, MouseEventArgs e)
        {

            int linha_select = listBox_Write_values.SelectedIndex;
            if (linha_select >= 0)
            {
                // clear de index element select with duble click 
                listBox_Write_values.Items.RemoveAt(linha_select);
            }
            else
            {
                // clear all 
                listBox_Write_values.Items.Clear();
            }

            
        }


        




    }
}
