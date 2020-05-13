using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace SerialPortListener
{
    public partial class EditPopUp : Form
    {
        public static string textbox_text = "";

        public EditPopUp()
        {
            InitializeComponent();
            editvaluebox.Text = textbox_text;
        }

        

        private void save_button_Click(object sender, EventArgs e)
        {
            if (editvaluebox.Text.Length > 0)
            {
                float tmp = float.Parse(editvaluebox.Text, CultureInfo.InvariantCulture.NumberFormat);
                tmp = tmp * 1000;
                int itmp = (Int16)tmp;
                string extrachars = "";
                if(itmp<1000)
                {
                    extrachars += "0";
                }
                if (itmp < 100)
                {
                    extrachars += "0";
                }
                if (itmp < 10)
                {
                    extrachars += "0";
                }
                if (itmp > 9999)
                {
                    itmp = 9999;
                }


            }
            
            this.Close();
        }

        private void cancel_button_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
