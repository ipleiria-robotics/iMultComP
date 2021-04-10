using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace App_Industry_comu
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        private void Btn_modbus_win_Click(object sender, EventArgs e)
        {
            modbus_win mod_win_form = new modbus_win();
            mod_win_form.Show(); 
        }

        private void but_board_Click(object sender, EventArgs e)
        {
            Board_comunication board_comunication = new Board_comunication();
            board_comunication.Show(); 
        }
    }
}
