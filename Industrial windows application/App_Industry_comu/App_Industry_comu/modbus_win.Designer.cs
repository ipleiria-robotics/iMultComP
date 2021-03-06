
namespace App_Industry_comu
{
    partial class modbus_win
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.textBox_IP = new System.Windows.Forms.TextBox();
            this.label_Server_port = new System.Windows.Forms.Label();
            this.textBox_port = new System.Windows.Forms.TextBox();
            this.label_baudrate = new System.Windows.Forms.Label();
            this.label_sever_ip = new System.Windows.Forms.Label();
            this.textBox_Slave_ID = new System.Windows.Forms.TextBox();
            this.textBox_Baud = new System.Windows.Forms.TextBox();
            this.comboBox_stopbits = new System.Windows.Forms.ComboBox();
            this.label_stop_bit = new System.Windows.Forms.Label();
            this.label_Parity = new System.Windows.Forms.Label();
            this.label_slave_ID = new System.Windows.Forms.Label();
            this.label_COM_port = new System.Windows.Forms.Label();
            this.comboBox_parity = new System.Windows.Forms.ComboBox();
            this.comboBox_Serial_port = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.Combox_type_comunication = new System.Windows.Forms.ComboBox();
            this.Btn_connect = new System.Windows.Forms.Button();
            this.Btn_disconnect = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.listBox_Write_values = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_write_address = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_prepere_coils = new System.Windows.Forms.TextBox();
            this.textBox_prepere_register = new System.Windows.Forms.TextBox();
            this.Btn_prepere_coils = new System.Windows.Forms.Button();
            this.Btn_prepere_registers = new System.Windows.Forms.Button();
            this.Btn_write_single_coils = new System.Windows.Forms.Button();
            this.Btn_write_single_registers = new System.Windows.Forms.Button();
            this.Bnt_write_multi_coils = new System.Windows.Forms.Button();
            this.Btn_write_milti_register = new System.Windows.Forms.Button();
            this.textBox_information = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.Btn_connect);
            this.panel1.Controls.Add(this.Btn_disconnect);
            this.panel1.Controls.Add(this.textBox_IP);
            this.panel1.Controls.Add(this.label_Server_port);
            this.panel1.Controls.Add(this.textBox_port);
            this.panel1.Controls.Add(this.label_baudrate);
            this.panel1.Controls.Add(this.label_sever_ip);
            this.panel1.Controls.Add(this.textBox_Slave_ID);
            this.panel1.Controls.Add(this.textBox_Baud);
            this.panel1.Controls.Add(this.comboBox_stopbits);
            this.panel1.Controls.Add(this.label_stop_bit);
            this.panel1.Controls.Add(this.label_Parity);
            this.panel1.Controls.Add(this.label_slave_ID);
            this.panel1.Controls.Add(this.label_COM_port);
            this.panel1.Controls.Add(this.comboBox_parity);
            this.panel1.Controls.Add(this.comboBox_Serial_port);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.Combox_type_comunication);
            this.panel1.Location = new System.Drawing.Point(12, 5);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1043, 104);
            this.panel1.TabIndex = 0;
            // 
            // textBox_IP
            // 
            this.textBox_IP.Location = new System.Drawing.Point(384, 26);
            this.textBox_IP.Name = "textBox_IP";
            this.textBox_IP.Size = new System.Drawing.Size(127, 20);
            this.textBox_IP.TabIndex = 15;
            this.textBox_IP.Text = "127.0.0.1";
            this.textBox_IP.TextChanged += new System.EventHandler(this.textBox_IP_TextChanged);
            // 
            // label_Server_port
            // 
            this.label_Server_port.AutoSize = true;
            this.label_Server_port.Location = new System.Drawing.Point(560, 10);
            this.label_Server_port.Name = "label_Server_port";
            this.label_Server_port.Size = new System.Drawing.Size(60, 13);
            this.label_Server_port.TabIndex = 13;
            this.label_Server_port.Text = "Server Port";
            // 
            // textBox_port
            // 
            this.textBox_port.Location = new System.Drawing.Point(555, 28);
            this.textBox_port.Name = "textBox_port";
            this.textBox_port.Size = new System.Drawing.Size(61, 20);
            this.textBox_port.TabIndex = 14;
            this.textBox_port.Text = "502";
            this.textBox_port.TextChanged += new System.EventHandler(this.textBox_port_TextChanged);
            // 
            // label_baudrate
            // 
            this.label_baudrate.Location = new System.Drawing.Point(649, 10);
            this.label_baudrate.Name = "label_baudrate";
            this.label_baudrate.Size = new System.Drawing.Size(65, 15);
            this.label_baudrate.TabIndex = 16;
            this.label_baudrate.Text = "BaudRate";
            // 
            // label_sever_ip
            // 
            this.label_sever_ip.AutoSize = true;
            this.label_sever_ip.Location = new System.Drawing.Point(422, 10);
            this.label_sever_ip.Name = "label_sever_ip";
            this.label_sever_ip.Size = new System.Drawing.Size(51, 13);
            this.label_sever_ip.TabIndex = 12;
            this.label_sever_ip.Text = "Server-IP";
            // 
            // textBox_Slave_ID
            // 
            this.textBox_Slave_ID.Location = new System.Drawing.Point(555, 27);
            this.textBox_Slave_ID.Name = "textBox_Slave_ID";
            this.textBox_Slave_ID.Size = new System.Drawing.Size(61, 20);
            this.textBox_Slave_ID.TabIndex = 11;
            this.textBox_Slave_ID.Text = "1";
            this.textBox_Slave_ID.TextChanged += new System.EventHandler(this.textBox_Slave_ID_TextChanged);
            // 
            // textBox_Baud
            // 
            this.textBox_Baud.Location = new System.Drawing.Point(652, 28);
            this.textBox_Baud.Name = "textBox_Baud";
            this.textBox_Baud.Size = new System.Drawing.Size(61, 20);
            this.textBox_Baud.TabIndex = 10;
            this.textBox_Baud.Text = "9600";
            this.textBox_Baud.TextChanged += new System.EventHandler(this.textBox_Baud_TextChanged);
            // 
            // comboBox_stopbits
            // 
            this.comboBox_stopbits.FormattingEnabled = true;
            this.comboBox_stopbits.Items.AddRange(new object[] {
            "1",
            "1.5",
            "2"});
            this.comboBox_stopbits.Location = new System.Drawing.Point(820, 28);
            this.comboBox_stopbits.Name = "comboBox_stopbits";
            this.comboBox_stopbits.Size = new System.Drawing.Size(63, 21);
            this.comboBox_stopbits.TabIndex = 9;
            this.comboBox_stopbits.SelectedIndexChanged += new System.EventHandler(this.comboBox_stopbits_SelectedIndexChanged);
            // 
            // label_stop_bit
            // 
            this.label_stop_bit.AutoSize = true;
            this.label_stop_bit.Location = new System.Drawing.Point(828, 12);
            this.label_stop_bit.Name = "label_stop_bit";
            this.label_stop_bit.Size = new System.Drawing.Size(45, 13);
            this.label_stop_bit.TabIndex = 8;
            this.label_stop_bit.Text = "Stopbits";
            // 
            // label_Parity
            // 
            this.label_Parity.AutoSize = true;
            this.label_Parity.Location = new System.Drawing.Point(746, 12);
            this.label_Parity.Name = "label_Parity";
            this.label_Parity.Size = new System.Drawing.Size(33, 13);
            this.label_Parity.TabIndex = 7;
            this.label_Parity.Text = "Parity";
            // 
            // label_slave_ID
            // 
            this.label_slave_ID.AutoSize = true;
            this.label_slave_ID.Location = new System.Drawing.Point(565, 10);
            this.label_slave_ID.Name = "label_slave_ID";
            this.label_slave_ID.Size = new System.Drawing.Size(48, 13);
            this.label_slave_ID.TabIndex = 6;
            this.label_slave_ID.Text = "Slave ID";
            // 
            // label_COM_port
            // 
            this.label_COM_port.AutoSize = true;
            this.label_COM_port.Location = new System.Drawing.Point(422, 10);
            this.label_COM_port.Name = "label_COM_port";
            this.label_COM_port.Size = new System.Drawing.Size(53, 13);
            this.label_COM_port.TabIndex = 5;
            this.label_COM_port.Text = "COM-Port";
            // 
            // comboBox_parity
            // 
            this.comboBox_parity.FormattingEnabled = true;
            this.comboBox_parity.Items.AddRange(new object[] {
            "Even",
            "Odd",
            "None"});
            this.comboBox_parity.Location = new System.Drawing.Point(732, 28);
            this.comboBox_parity.Name = "comboBox_parity";
            this.comboBox_parity.Size = new System.Drawing.Size(63, 21);
            this.comboBox_parity.TabIndex = 4;
            this.comboBox_parity.SelectedIndexChanged += new System.EventHandler(this.comboBox_parity_SelectedIndexChanged);
            // 
            // comboBox_Serial_port
            // 
            this.comboBox_Serial_port.FormattingEnabled = true;
            this.comboBox_Serial_port.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "COM10"});
            this.comboBox_Serial_port.Location = new System.Drawing.Point(384, 26);
            this.comboBox_Serial_port.Name = "comboBox_Serial_port";
            this.comboBox_Serial_port.Size = new System.Drawing.Size(127, 21);
            this.comboBox_Serial_port.TabIndex = 2;
            this.comboBox_Serial_port.SelectedIndexChanged += new System.EventHandler(this.comboBox_Serial_port_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label1.Location = new System.Drawing.Point(3, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(176, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "Type Modbus Comunication";
            // 
            // Combox_type_comunication
            // 
            this.Combox_type_comunication.FormattingEnabled = true;
            this.Combox_type_comunication.Items.AddRange(new object[] {
            "ModbusTCP (Ethernet)",
            "ModbusRTU (Serial)"});
            this.Combox_type_comunication.Location = new System.Drawing.Point(182, 27);
            this.Combox_type_comunication.Name = "Combox_type_comunication";
            this.Combox_type_comunication.Size = new System.Drawing.Size(164, 21);
            this.Combox_type_comunication.TabIndex = 0;
            this.Combox_type_comunication.SelectedIndexChanged += new System.EventHandler(this.Combox_type_comunication_SelectedIndexChanged);
            // 
            // Btn_connect
            // 
            this.Btn_connect.Location = new System.Drawing.Point(384, 76);
            this.Btn_connect.Name = "Btn_connect";
            this.Btn_connect.Size = new System.Drawing.Size(107, 27);
            this.Btn_connect.TabIndex = 1;
            this.Btn_connect.Text = "Connect";
            this.Btn_connect.UseVisualStyleBackColor = true;
            this.Btn_connect.Click += new System.EventHandler(this.Btn_connect_Click);
            // 
            // Btn_disconnect
            // 
            this.Btn_disconnect.Location = new System.Drawing.Point(510, 76);
            this.Btn_disconnect.Name = "Btn_disconnect";
            this.Btn_disconnect.Size = new System.Drawing.Size(110, 27);
            this.Btn_disconnect.TabIndex = 2;
            this.Btn_disconnect.Text = "Disconnect";
            this.Btn_disconnect.UseVisualStyleBackColor = true;
            this.Btn_disconnect.Click += new System.EventHandler(this.Btn_disconnect_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(576, 150);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(278, 225);
            this.listBox1.TabIndex = 1;
            // 
            // listBox_Write_values
            // 
            this.listBox_Write_values.FormattingEnabled = true;
            this.listBox_Write_values.Location = new System.Drawing.Point(179, 150);
            this.listBox_Write_values.Name = "listBox_Write_values";
            this.listBox_Write_values.Size = new System.Drawing.Size(278, 225);
            this.listBox_Write_values.TabIndex = 2;
            this.listBox_Write_values.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listBox_Write_values_MouseDoubleClick);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(204, 123);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(224, 24);
            this.label2.TabIndex = 3;
            this.label2.Text = "Write Values to Server ";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(599, 123);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(225, 24);
            this.label3.TabIndex = 4;
            this.label3.Text = "Read Values to Server ";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(176, 392);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(84, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Starting Address";
            // 
            // textBox_write_address
            // 
            this.textBox_write_address.BackColor = System.Drawing.Color.Wheat;
            this.textBox_write_address.Location = new System.Drawing.Point(266, 389);
            this.textBox_write_address.Name = "textBox_write_address";
            this.textBox_write_address.Size = new System.Drawing.Size(64, 20);
            this.textBox_write_address.TabIndex = 6;
            this.textBox_write_address.Text = "1";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(176, 422);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(69, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Prepere Coils";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(169, 453);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(91, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Prepere Registers";
            // 
            // textBox_prepere_coils
            // 
            this.textBox_prepere_coils.BackColor = System.Drawing.Color.Wheat;
            this.textBox_prepere_coils.Location = new System.Drawing.Point(266, 415);
            this.textBox_prepere_coils.Name = "textBox_prepere_coils";
            this.textBox_prepere_coils.Size = new System.Drawing.Size(64, 20);
            this.textBox_prepere_coils.TabIndex = 9;
            this.textBox_prepere_coils.Text = "FALSE";
            this.textBox_prepere_coils.DoubleClick += new System.EventHandler(this.textBox_prepere_coils_DoubleClick);
            // 
            // textBox_prepere_register
            // 
            this.textBox_prepere_register.BackColor = System.Drawing.Color.Wheat;
            this.textBox_prepere_register.Location = new System.Drawing.Point(266, 447);
            this.textBox_prepere_register.Name = "textBox_prepere_register";
            this.textBox_prepere_register.Size = new System.Drawing.Size(64, 20);
            this.textBox_prepere_register.TabIndex = 10;
            this.textBox_prepere_register.Text = "0";
            // 
            // Btn_prepere_coils
            // 
            this.Btn_prepere_coils.Location = new System.Drawing.Point(336, 415);
            this.Btn_prepere_coils.Name = "Btn_prepere_coils";
            this.Btn_prepere_coils.Size = new System.Drawing.Size(75, 20);
            this.Btn_prepere_coils.TabIndex = 11;
            this.Btn_prepere_coils.Text = "Write";
            this.Btn_prepere_coils.UseVisualStyleBackColor = true;
            this.Btn_prepere_coils.Click += new System.EventHandler(this.Btn_prepere_coils_Click);
            // 
            // Btn_prepere_registers
            // 
            this.Btn_prepere_registers.Location = new System.Drawing.Point(336, 446);
            this.Btn_prepere_registers.Name = "Btn_prepere_registers";
            this.Btn_prepere_registers.Size = new System.Drawing.Size(75, 20);
            this.Btn_prepere_registers.TabIndex = 12;
            this.Btn_prepere_registers.Text = "Write";
            this.Btn_prepere_registers.UseVisualStyleBackColor = true;
            this.Btn_prepere_registers.Click += new System.EventHandler(this.Btn_prepere_registers_Click);
            // 
            // Btn_write_single_coils
            // 
            this.Btn_write_single_coils.Location = new System.Drawing.Point(13, 150);
            this.Btn_write_single_coils.Name = "Btn_write_single_coils";
            this.Btn_write_single_coils.Size = new System.Drawing.Size(157, 25);
            this.Btn_write_single_coils.TabIndex = 13;
            this.Btn_write_single_coils.Text = "Write Single Coil - FC5 ";
            this.Btn_write_single_coils.UseVisualStyleBackColor = true;
            this.Btn_write_single_coils.Click += new System.EventHandler(this.Btn_write_single_coils_Click);
            // 
            // Btn_write_single_registers
            // 
            this.Btn_write_single_registers.Location = new System.Drawing.Point(13, 190);
            this.Btn_write_single_registers.Name = "Btn_write_single_registers";
            this.Btn_write_single_registers.Size = new System.Drawing.Size(157, 25);
            this.Btn_write_single_registers.TabIndex = 14;
            this.Btn_write_single_registers.Text = "Write Single Register FC6 ";
            this.Btn_write_single_registers.UseVisualStyleBackColor = true;
            this.Btn_write_single_registers.Click += new System.EventHandler(this.Btn_write_single_registers_Click);
            // 
            // Bnt_write_multi_coils
            // 
            this.Bnt_write_multi_coils.Location = new System.Drawing.Point(13, 232);
            this.Bnt_write_multi_coils.Name = "Bnt_write_multi_coils";
            this.Bnt_write_multi_coils.Size = new System.Drawing.Size(157, 25);
            this.Bnt_write_multi_coils.TabIndex = 15;
            this.Bnt_write_multi_coils.Text = "Write Multiple Coils - FC15";
            this.Bnt_write_multi_coils.UseVisualStyleBackColor = true;
            this.Bnt_write_multi_coils.Click += new System.EventHandler(this.Bnt_write_multi_coils_Click);
            // 
            // Btn_write_milti_register
            // 
            this.Btn_write_milti_register.Location = new System.Drawing.Point(13, 276);
            this.Btn_write_milti_register.Name = "Btn_write_milti_register";
            this.Btn_write_milti_register.Size = new System.Drawing.Size(157, 25);
            this.Btn_write_milti_register.TabIndex = 16;
            this.Btn_write_milti_register.Text = "Write Multiple Register - FC16";
            this.Btn_write_milti_register.UseVisualStyleBackColor = true;
            this.Btn_write_milti_register.Click += new System.EventHandler(this.Btn_write_milti_register_Click);
            // 
            // textBox_information
            // 
            this.textBox_information.Location = new System.Drawing.Point(12, 473);
            this.textBox_information.Multiline = true;
            this.textBox_information.Name = "textBox_information";
            this.textBox_information.Size = new System.Drawing.Size(1033, 84);
            this.textBox_information.TabIndex = 17;
            // 
            // modbus_win
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Menu;
            this.ClientSize = new System.Drawing.Size(1057, 558);
            this.Controls.Add(this.textBox_information);
            this.Controls.Add(this.Btn_write_milti_register);
            this.Controls.Add(this.Bnt_write_multi_coils);
            this.Controls.Add(this.Btn_write_single_registers);
            this.Controls.Add(this.Btn_write_single_coils);
            this.Controls.Add(this.Btn_prepere_registers);
            this.Controls.Add(this.Btn_prepere_coils);
            this.Controls.Add(this.textBox_prepere_register);
            this.Controls.Add(this.textBox_prepere_coils);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_write_address);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.listBox_Write_values);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.panel1);
            this.Name = "modbus_win";
            this.Text = "modbus_win";
            this.Load += new System.EventHandler(this.modbus_win_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox Combox_type_comunication;
        private System.Windows.Forms.Label label_baudrate;
        private System.Windows.Forms.Label label_Parity;
        private System.Windows.Forms.Label label_slave_ID;
        private System.Windows.Forms.Label label_COM_port;
        private System.Windows.Forms.ComboBox comboBox_parity;
        private System.Windows.Forms.ComboBox comboBox_Serial_port;
        private System.Windows.Forms.TextBox textBox_Slave_ID;
        private System.Windows.Forms.TextBox textBox_Baud;
        private System.Windows.Forms.ComboBox comboBox_stopbits;
        private System.Windows.Forms.Label label_stop_bit;
        private System.Windows.Forms.Label label_Server_port;
        private System.Windows.Forms.Label label_sever_ip;
        private System.Windows.Forms.TextBox textBox_IP;
        private System.Windows.Forms.TextBox textBox_port;
        private System.Windows.Forms.Button Btn_connect;
        private System.Windows.Forms.Button Btn_disconnect;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.ListBox listBox_Write_values;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_write_address;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_prepere_coils;
        private System.Windows.Forms.TextBox textBox_prepere_register;
        private System.Windows.Forms.Button Btn_prepere_coils;
        private System.Windows.Forms.Button Btn_prepere_registers;
        private System.Windows.Forms.Button Btn_write_single_coils;
        private System.Windows.Forms.Button Btn_write_single_registers;
        private System.Windows.Forms.Button Bnt_write_multi_coils;
        private System.Windows.Forms.Button Btn_write_milti_register;
        private System.Windows.Forms.TextBox textBox_information;
    }
}