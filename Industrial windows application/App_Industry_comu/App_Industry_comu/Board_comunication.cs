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
using System.Threading;

namespace App_Industry_comu
{
    public partial class Board_comunication : Form
    {

        Com_Maneger com = new Com_Maneger();
        public Board_comunication()
        {
            
            InitializeComponent();
            Thread.Sleep(2000);
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
            try
            {
                com.PortName = combox_portt.Items[combox_portt.SelectedIndex].ToString();
                com.Open_Port();
            }
            catch (Exception ex)
            {

            }
            
            
        }

         private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            //richTextBox1.Clear(); 
        }

        private void richTextBox1_DoubleClick(object sender, EventArgs e)
        {
            richTextBox1.Clear();
        }

        private void but_task_init_Click(object sender, EventArgs e)
        {
            if(comboBox_protocol.SelectedIndex == 0)
            {
                com.Write_Data("cat");
            }else if(comboBox_protocol.SelectedIndex == 2)
            {

            }
        }

        private void btn_finnish_Click(object sender, EventArgs e)
        {
            if (comboBox_protocol.SelectedIndex == 0)
            {
                com.Write_Data("ecat");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            com.wwrite_data("\n");
            com.wwrite_data("rdd\n");
          //  com.wwrite_data("\n");
            
         //   if(tt == "rdd" ){
        //        return;
         //   }

            txt_rd_ether.Text = com.Read_comPort(); ;



            Thread.Sleep(1000);
            com.wwrite_data("\n");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            com.wwrite_data("wdd\n");
            string x = txbox_write_ethercat.Text;
            com.wwrite_data(x+"\n");

        }
    }
}
