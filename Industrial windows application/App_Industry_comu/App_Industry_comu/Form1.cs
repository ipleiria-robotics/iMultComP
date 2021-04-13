using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace App_Industry_comu
{
    public partial class Form1 : Form
    {
        
       

        public Form1()
        {
            InitializeComponent();
           
        }
        

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

       

        private void modbusSlaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            modbus_win mod_win_form = new modbus_win();
            mod_win_form.Show();
        }

        private void iCEAm3359InfoToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void datasheetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Board_comunication board_comunication = new Board_comunication();
            board_comunication.Show();
        }

        private void exitToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void datasheetToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            Process.Start("https://www.ti.com/lit/ug/spruip3/spruip3.pdf?ts=1618308239660");
        }
    }
}
