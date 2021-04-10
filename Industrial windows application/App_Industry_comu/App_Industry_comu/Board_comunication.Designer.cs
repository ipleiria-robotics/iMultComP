
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
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.context_menu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.combox_portt = new System.Windows.Forms.ComboBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.button_open = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.button_cclink = new System.Windows.Forms.Button();
            this.button_finish_cclink = new System.Windows.Forms.Button();
            this.button_exit_APP = new System.Windows.Forms.Button();
            this.modbus_finish_task = new System.Windows.Forms.Button();
            this.modbus_create_task = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.context_menu.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label1.Location = new System.Drawing.Point(32, 35);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 20);
            this.label1.TabIndex = 2;
            this.label1.Text = "Serial Port";
            // 
            // context_menu
            // 
            this.context_menu.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.context_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.saveToolStripMenuItem,
            this.newToolStripMenuItem});
            this.context_menu.Name = "context_menu";
            this.context_menu.ShowCheckMargin = true;
            this.context_menu.Size = new System.Drawing.Size(137, 76);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(136, 24);
            this.toolStripMenuItem1.Text = "Close";
            this.toolStripMenuItem1.Click += new System.EventHandler(this.toolStripMenuItem1_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(136, 24);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(136, 24);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
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
            this.richTextBox1.Location = new System.Drawing.Point(661, 25);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(685, 493);
            this.richTextBox1.TabIndex = 5;
            this.richTextBox1.Text = "";
            // 
            // button_open
            // 
            this.button_open.Location = new System.Drawing.Point(323, 26);
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
            this.panel1.Controls.Add(this.button_exit_APP);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.button_open);
            this.panel1.Controls.Add(this.combox_portt);
            this.panel1.Location = new System.Drawing.Point(24, 25);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(631, 493);
            this.panel1.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label2.Location = new System.Drawing.Point(14, 49);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 20);
            this.label2.TabIndex = 7;
            this.label2.Text = "CC Link Protocol";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.modbus_finish_task);
            this.panel2.Controls.Add(this.modbus_create_task);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Controls.Add(this.button_finish_cclink);
            this.panel2.Controls.Add(this.button_cclink);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Location = new System.Drawing.Point(18, 79);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(594, 390);
            this.panel2.TabIndex = 7;
            // 
            // button_cclink
            // 
            this.button_cclink.Location = new System.Drawing.Point(171, 48);
            this.button_cclink.Name = "button_cclink";
            this.button_cclink.Size = new System.Drawing.Size(149, 23);
            this.button_cclink.TabIndex = 8;
            this.button_cclink.Text = "Create ";
            this.button_cclink.UseVisualStyleBackColor = true;
            this.button_cclink.UseWaitCursor = true;
            this.button_cclink.Click += new System.EventHandler(this.button_cclink_Click);
            // 
            // button_finish_cclink
            // 
            this.button_finish_cclink.Location = new System.Drawing.Point(349, 48);
            this.button_finish_cclink.Name = "button_finish_cclink";
            this.button_finish_cclink.Size = new System.Drawing.Size(149, 23);
            this.button_finish_cclink.TabIndex = 9;
            this.button_finish_cclink.Text = "finish";
            this.button_finish_cclink.UseVisualStyleBackColor = true;
            this.button_finish_cclink.UseWaitCursor = true;
            this.button_finish_cclink.Click += new System.EventHandler(this.button_finish_cclink_Click);
            // 
            // button_exit_APP
            // 
            this.button_exit_APP.Location = new System.Drawing.Point(478, 27);
            this.button_exit_APP.Name = "button_exit_APP";
            this.button_exit_APP.Size = new System.Drawing.Size(149, 38);
            this.button_exit_APP.TabIndex = 8;
            this.button_exit_APP.Text = "Exit App";
            this.button_exit_APP.UseVisualStyleBackColor = true;
            this.button_exit_APP.UseWaitCursor = true;
            this.button_exit_APP.Click += new System.EventHandler(this.button_exit_APP_Click);
            // 
            // modbus_finish_task
            // 
            this.modbus_finish_task.Location = new System.Drawing.Point(349, 110);
            this.modbus_finish_task.Name = "modbus_finish_task";
            this.modbus_finish_task.Size = new System.Drawing.Size(149, 23);
            this.modbus_finish_task.TabIndex = 12;
            this.modbus_finish_task.Text = "finish";
            this.modbus_finish_task.UseVisualStyleBackColor = true;
            this.modbus_finish_task.UseWaitCursor = true;
            this.modbus_finish_task.Click += new System.EventHandler(this.modbus_finish_task_Click);
            // 
            // modbus_create_task
            // 
            this.modbus_create_task.Location = new System.Drawing.Point(171, 110);
            this.modbus_create_task.Name = "modbus_create_task";
            this.modbus_create_task.Size = new System.Drawing.Size(149, 23);
            this.modbus_create_task.TabIndex = 11;
            this.modbus_create_task.Text = "Create ";
            this.modbus_create_task.UseVisualStyleBackColor = true;
            this.modbus_create_task.UseWaitCursor = true;
            this.modbus_create_task.Click += new System.EventHandler(this.modbus_create_task_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label3.Location = new System.Drawing.Point(14, 111);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(128, 20);
            this.label3.TabIndex = 10;
            this.label3.Text = "Modbus Protocol";
            // 
            // Board_comunication
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1358, 572);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.richTextBox1);
            this.Name = "Board_comunication";
            this.Text = "Board_comunication";
            this.Load += new System.EventHandler(this.Board_comunication_Load);
            this.context_menu.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ContextMenuStrip context_menu;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ComboBox combox_portt;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button button_open;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button button_cclink;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button_finish_cclink;
        private System.Windows.Forms.Button button_exit_APP;
        private System.Windows.Forms.Button modbus_finish_task;
        private System.Windows.Forms.Button modbus_create_task;
        private System.Windows.Forms.Label label3;
    }
}