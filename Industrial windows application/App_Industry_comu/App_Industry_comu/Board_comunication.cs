using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using App_Industry_comu.Control_board;

namespace App_Industry_comu
{
    public partial class Board_comunication : Form
    {

        Com_Maneger com = new Com_Maneger();
        public Board_comunication()
        {
            
            InitializeComponent();
        }
        private void Board_comunication_Load(object sender, EventArgs e)
        {
            com.SetPortNameValues(combox_portt);
            com.Parity = "None";
            com.Baudrate = "115200";
            com.StopBits = "One";
            com.DataBits = "8";
            com.DisplayWindow = richTextBox1;

            
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.Close(); 
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Board_comunication bb_new = new Board_comunication();
            bb_new.Show(); 
        }

        private void button_open_Click(object sender, EventArgs e)
        {
            com.PortName = combox_portt.Items[combox_portt.SelectedIndex].ToString();
            com.Open_Port(); 
            
        }


        private void button_cclink_Click(object sender, EventArgs e)
        {
            com.Write_Data("cc");
        }

        private void button_finish_cclink_Click(object sender, EventArgs e)
        {
            com.Write_Data("ecc");
        }

        private void button_exit_APP_Click(object sender, EventArgs e)
        {
            com.Write_Data("exit");
        }

        private void modbus_create_task_Click(object sender, EventArgs e)
        {
            com.Write_Data("mb");
        }

        private void modbus_finish_task_Click(object sender, EventArgs e)
        {
            com.Write_Data("emb");
        }


    }
}
