
namespace App_Industry_comu
{
    partial class Board_comunication
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
            this.label1 = new System.Windows.Forms.Label();
            this.combox_portt = new System.Windows.Forms.ComboBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.button_open = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.txt_rd_ether = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.btn_finnish = new System.Windows.Forms.Button();
            this.but_task_init = new System.Windows.Forms.Button();
            this.comboBox_protocol = new System.Windows.Forms.ComboBox();
            this.button2 = new System.Windows.Forms.Button();
            this.txbox_write_ethercat = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label1.Location = new System.Drawing.Point(32, 34);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 20);
            this.label1.TabIndex = 2;
            this.label1.Text = "Serial Port";
            // 
            // combox_portt
            // 
            this.combox_portt.FormattingEnabled = true;
            this.combox_portt.Items.AddRange(new object[] {
            "ModbusTCP (Ethernet)",
            "ModbusRTU (Serial)"});
            this.combox_portt.Location = new System.Drawing.Point(133, 34);
            this.combox_portt.Margin = new System.Windows.Forms.Padding(4);
            this.combox_portt.Name = "combox_portt";
            this.combox_portt.Size = new System.Drawing.Size(167, 24);
            this.combox_portt.TabIndex = 4;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(656, 24);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(685, 493);
            this.richTextBox1.TabIndex = 5;
            this.richTextBox1.Text = "";
            this.richTextBox1.TextChanged += new System.EventHandler(this.richTextBox1_TextChanged);
            this.richTextBox1.DoubleClick += new System.EventHandler(this.richTextBox1_DoubleClick);
            // 
            // button_open
            // 
            this.button_open.Location = new System.Drawing.Point(323, 26);
            this.button_open.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_open.Name = "button_open";
            this.button_open.Size = new System.Drawing.Size(149, 39);
            this.button_open.TabIndex = 6;
            this.button_open.Text = "Open";
            this.button_open.UseVisualStyleBackColor = true;
            this.button_open.UseWaitCursor = true;
            this.button_open.Click += new System.EventHandler(this.button_open_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.button_open);
            this.panel1.Controls.Add(this.combox_portt);
            this.panel1.Location = new System.Drawing.Point(24, 25);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(631, 494);
            this.panel1.TabIndex = 7;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.txbox_write_ethercat);
            this.panel2.Controls.Add(this.button2);
            this.panel2.Controls.Add(this.txt_rd_ether);
            this.panel2.Controls.Add(this.button1);
            this.panel2.Controls.Add(this.btn_finnish);
            this.panel2.Controls.Add(this.but_task_init);
            this.panel2.Controls.Add(this.comboBox_protocol);
            this.panel2.Location = new System.Drawing.Point(19, 79);
            this.panel2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(595, 390);
            this.panel2.TabIndex = 7;
            // 
            // txt_rd_ether
            // 
            this.txt_rd_ether.Location = new System.Drawing.Point(304, 138);
            this.txt_rd_ether.Name = "txt_rd_ether";
            this.txt_rd_ether.ReadOnly = true;
            this.txt_rd_ether.Size = new System.Drawing.Size(188, 22);
            this.txt_rd_ether.TabIndex = 40;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(68, 133);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(227, 33);
            this.button1.TabIndex = 37;
            this.button1.Text = "Read Byte Ethercat";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btn_finnish
            // 
            this.btn_finnish.Location = new System.Drawing.Point(428, 36);
            this.btn_finnish.Name = "btn_finnish";
            this.btn_finnish.Size = new System.Drawing.Size(127, 38);
            this.btn_finnish.TabIndex = 36;
            this.btn_finnish.Text = "Finish";
            this.btn_finnish.UseVisualStyleBackColor = true;
            this.btn_finnish.Click += new System.EventHandler(this.btn_finnish_Click);
            // 
            // but_task_init
            // 
            this.but_task_init.Location = new System.Drawing.Point(290, 36);
            this.but_task_init.Name = "but_task_init";
            this.but_task_init.Size = new System.Drawing.Size(132, 38);
            this.but_task_init.TabIndex = 35;
            this.but_task_init.Text = "Init";
            this.but_task_init.UseVisualStyleBackColor = true;
            this.but_task_init.Click += new System.EventHandler(this.but_task_init_Click);
            // 
            // comboBox_protocol
            // 
            this.comboBox_protocol.FormattingEnabled = true;
            this.comboBox_protocol.Items.AddRange(new object[] {
            "Ethercat",
            "Profinet",
            "Profibus",
            "CC_Link",
            "Modbus"});
            this.comboBox_protocol.Location = new System.Drawing.Point(17, 44);
            this.comboBox_protocol.Name = "comboBox_protocol";
            this.comboBox_protocol.Size = new System.Drawing.Size(250, 24);
            this.comboBox_protocol.TabIndex = 34;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(68, 190);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(227, 33);
            this.button2.TabIndex = 41;
            this.button2.Text = "Wrire Byte Ethercat";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // txbox_write_ethercat
            // 
            this.txbox_write_ethercat.Location = new System.Drawing.Point(304, 195);
            this.txbox_write_ethercat.Name = "txbox_write_ethercat";
            this.txbox_write_ethercat.Size = new System.Drawing.Size(188, 22);
            this.txbox_write_ethercat.TabIndex = 42;
            // 
            // Board_comunication
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1085, 457);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.richTextBox1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Board_comunication";
            this.Text = "Board_comunication";
            this.Load += new System.EventHandler(this.Board_comunication_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox combox_portt;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button button_open;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button but_task_init;
        private System.Windows.Forms.ComboBox comboBox_protocol;
        private System.Windows.Forms.Button btn_finnish;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txt_rd_ether;
        private System.Windows.Forms.TextBox txbox_write_ethercat;
        private System.Windows.Forms.Button button2;
    }
}