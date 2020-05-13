using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SerialPortListener.Serial;
using System.IO;
using System.Globalization;

namespace SerialPortListener
{
    public partial class MainForm : Form
    {
        int[,] dataArray = new int[15, 15];
        TextBox[,] myTextBox;
        SerialPortManager _spManager;
        public MainForm()
        {
            InitializeComponent();
            myTextBox = new TextBox[15, 15];
            UserInitialization();
            int x =0; int y = 0;
            myTextBox[y, x] = textBox1; x++;
            myTextBox[y, x] = textBox2; x++;
            myTextBox[y, x] = textBox3; x++;
            myTextBox[y, x] = textBox4; x++;
            myTextBox[y, x] = textBox5; x++;
            myTextBox[y, x] = textBox6; x++;
            myTextBox[y, x] = textBox7; x++;
            myTextBox[y, x] = textBox8; x++;
            myTextBox[y, x] = textBox9; x++;
            myTextBox[y, x] = textBox10; x++;
            myTextBox[y, x] = textBox11; x++;
            myTextBox[y, x] = textBox12; x++;
            myTextBox[y, x] = textBox13; x++;
            myTextBox[y, x] = textBox14; x++;
            myTextBox[y, x] = textBox15; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox16; x++;
            myTextBox[y, x] = textBox17; x++;
            myTextBox[y, x] = textBox18; x++;
            myTextBox[y, x] = textBox19; x++;
            myTextBox[y, x] = textBox20; x++;
            myTextBox[y, x] = textBox21; x++;
            myTextBox[y, x] = textBox22; x++;
            myTextBox[y, x] = textBox23; x++;
            myTextBox[y, x] = textBox24; x++;
            myTextBox[y, x] = textBox25; x++;
            myTextBox[y, x] = textBox26; x++;
            myTextBox[y, x] = textBox27; x++;
            myTextBox[y, x] = textBox28; x++;
            myTextBox[y, x] = textBox29; x++;
            myTextBox[y, x] = textBox30; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox31; x++;
            myTextBox[y, x] = textBox32; x++;
            myTextBox[y, x] = textBox33; x++;
            myTextBox[y, x] = textBox34; x++;
            myTextBox[y, x] = textBox35; x++;
            myTextBox[y, x] = textBox36; x++;
            myTextBox[y, x] = textBox37; x++;
            myTextBox[y, x] = textBox38; x++;
            myTextBox[y, x] = textBox39; x++;
            myTextBox[y, x] = textBox40; x++;
            myTextBox[y, x] = textBox41; x++;
            myTextBox[y, x] = textBox42; x++;
            myTextBox[y, x] = textBox43; x++;
            myTextBox[y, x] = textBox44; x++;
            myTextBox[y, x] = textBox45; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox46; x++;
            myTextBox[y, x] = textBox47; x++;
            myTextBox[y, x] = textBox48; x++;
            myTextBox[y, x] = textBox49; x++;
            myTextBox[y, x] = textBox50; x++;
            myTextBox[y, x] = textBox51; x++;
            myTextBox[y, x] = textBox52; x++;
            myTextBox[y, x] = textBox53; x++;
            myTextBox[y, x] = textBox54; x++;
            myTextBox[y, x] = textBox55; x++;
            myTextBox[y, x] = textBox56; x++;
            myTextBox[y, x] = textBox57; x++;
            myTextBox[y, x] = textBox58; x++;
            myTextBox[y, x] = textBox59; x++;
            myTextBox[y, x] = textBox60; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox61; x++;
            myTextBox[y, x] = textBox62; x++;
            myTextBox[y, x] = textBox63; x++;
            myTextBox[y, x] = textBox64; x++;
            myTextBox[y, x] = textBox65; x++;
            myTextBox[y, x] = textBox66; x++;
            myTextBox[y, x] = textBox67; x++;
            myTextBox[y, x] = textBox68; x++;
            myTextBox[y, x] = textBox69; x++;
            myTextBox[y, x] = textBox70; x++;
            myTextBox[y, x] = textBox71; x++;
            myTextBox[y, x] = textBox72; x++;
            myTextBox[y, x] = textBox73; x++;
            myTextBox[y, x] = textBox74; x++;
            myTextBox[y, x] = textBox75; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox76; x++;
            myTextBox[y, x] = textBox77; x++;
            myTextBox[y, x] = textBox78; x++;
            myTextBox[y, x] = textBox79; x++;
            myTextBox[y, x] = textBox80; x++;
            myTextBox[y, x] = textBox81; x++;
            myTextBox[y, x] = textBox82; x++;
            myTextBox[y, x] = textBox83; x++;
            myTextBox[y, x] = textBox84; x++;
            myTextBox[y, x] = textBox85; x++;
            myTextBox[y, x] = textBox86; x++;
            myTextBox[y, x] = textBox87; x++;
            myTextBox[y, x] = textBox88; x++;
            myTextBox[y, x] = textBox89; x++;
            myTextBox[y, x] = textBox90; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox91; x++;
            myTextBox[y, x] = textBox92; x++;
            myTextBox[y, x] = textBox93; x++;
            myTextBox[y, x] = textBox94; x++;
            myTextBox[y, x] = textBox95; x++;
            myTextBox[y, x] = textBox96; x++;
            myTextBox[y, x] = textBox97; x++;
            myTextBox[y, x] = textBox98; x++;
            myTextBox[y, x] = textBox99; x++;
            myTextBox[y, x] = textBox100; x++;
            myTextBox[y, x] = textBox101; x++;
            myTextBox[y, x] = textBox102; x++;
            myTextBox[y, x] = textBox103; x++;
            myTextBox[y, x] = textBox104; x++;
            myTextBox[y, x] = textBox105; x++;
            y++; x = 0;

            myTextBox[y, x] = textBox106; x++;
            myTextBox[y, x] = textBox107; x++;
            myTextBox[y, x] = textBox108; x++;
            myTextBox[y, x] = textBox109; x++;
            myTextBox[y, x] = textBox110; x++;
            myTextBox[y, x] = textBox111; x++;
            myTextBox[y, x] = textBox112; x++;
            myTextBox[y, x] = textBox113; x++;
            myTextBox[y, x] = textBox114; x++;
            myTextBox[y, x] = textBox115; x++;
            myTextBox[y, x] = textBox116; x++;
            myTextBox[y, x] = textBox117; x++;
            myTextBox[y, x] = textBox118; x++;
            myTextBox[y, x] = textBox119; x++;
            myTextBox[y, x] = textBox120; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox121; x++;
            myTextBox[y, x] = textBox122; x++;
            myTextBox[y, x] = textBox123; x++;
            myTextBox[y, x] = textBox124; x++;
            myTextBox[y, x] = textBox125; x++;
            myTextBox[y, x] = textBox126; x++;
            myTextBox[y, x] = textBox127; x++;
            myTextBox[y, x] = textBox128; x++;
            myTextBox[y, x] = textBox129; x++;
            myTextBox[y, x] = textBox130; x++;
            myTextBox[y, x] = textBox131; x++;
            myTextBox[y, x] = textBox132; x++;
            myTextBox[y, x] = textBox133; x++;
            myTextBox[y, x] = textBox134; x++;
            myTextBox[y, x] = textBox135; x++;
            y++; x = 0;

            myTextBox[y, x] = textBox136; x++;
            myTextBox[y, x] = textBox137; x++;
            myTextBox[y, x] = textBox138; x++;
            myTextBox[y, x] = textBox139; x++;
            myTextBox[y, x] = textBox140; x++;
            myTextBox[y, x] = textBox141; x++;
            myTextBox[y, x] = textBox142; x++;
            myTextBox[y, x] = textBox143; x++;
            myTextBox[y, x] = textBox144; x++;
            myTextBox[y, x] = textBox145; x++;
            myTextBox[y, x] = textBox146; x++;
            myTextBox[y, x] = textBox147; x++;
            myTextBox[y, x] = textBox148; x++;
            myTextBox[y, x] = textBox149; x++;
            myTextBox[y, x] = textBox150; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox151; x++;
            myTextBox[y, x] = textBox152; x++;
            myTextBox[y, x] = textBox153; x++;
            myTextBox[y, x] = textBox154; x++;
            myTextBox[y, x] = textBox155; x++;
            myTextBox[y, x] = textBox156; x++;
            myTextBox[y, x] = textBox157; x++;
            myTextBox[y, x] = textBox158; x++;
            myTextBox[y, x] = textBox159; x++;
            myTextBox[y, x] = textBox160; x++;
            myTextBox[y, x] = textBox161; x++;
            myTextBox[y, x] = textBox162; x++;
            myTextBox[y, x] = textBox163; x++;
            myTextBox[y, x] = textBox164; x++;
            myTextBox[y, x] = textBox165; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox166; x++;
            myTextBox[y, x] = textBox167; x++;
            myTextBox[y, x] = textBox168; x++;
            myTextBox[y, x] = textBox169; x++;
            myTextBox[y, x] = textBox170; x++;
            myTextBox[y, x] = textBox171; x++;
            myTextBox[y, x] = textBox172; x++;
            myTextBox[y, x] = textBox173; x++;
            myTextBox[y, x] = textBox174; x++;
            myTextBox[y, x] = textBox175; x++;
            myTextBox[y, x] = textBox176; x++;
            myTextBox[y, x] = textBox177; x++;
            myTextBox[y, x] = textBox178; x++;
            myTextBox[y, x] = textBox179; x++;
            myTextBox[y, x] = textBox180; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox181; x++;
            myTextBox[y, x] = textBox182; x++;
            myTextBox[y, x] = textBox183; x++;
            myTextBox[y, x] = textBox184; x++;
            myTextBox[y, x] = textBox185; x++;
            myTextBox[y, x] = textBox186; x++;
            myTextBox[y, x] = textBox187; x++;
            myTextBox[y, x] = textBox188; x++;
            myTextBox[y, x] = textBox189; x++;
            myTextBox[y, x] = textBox190; x++;
            myTextBox[y, x] = textBox191; x++;
            myTextBox[y, x] = textBox192; x++;
            myTextBox[y, x] = textBox193; x++;
            myTextBox[y, x] = textBox194; x++;
            myTextBox[y, x] = textBox195; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox196; x++;
            myTextBox[y, x] = textBox197; x++;
            myTextBox[y, x] = textBox198; x++;
            myTextBox[y, x] = textBox199; x++;
            myTextBox[y, x] = textBox200; x++;
            myTextBox[y, x] = textBox201; x++;
            myTextBox[y, x] = textBox202; x++;
            myTextBox[y, x] = textBox203; x++;
            myTextBox[y, x] = textBox204; x++;
            myTextBox[y, x] = textBox205; x++;
            myTextBox[y, x] = textBox206; x++;
            myTextBox[y, x] = textBox207; x++;
            myTextBox[y, x] = textBox208; x++;
            myTextBox[y, x] = textBox209; x++;
            myTextBox[y, x] = textBox210; x++;
            y++; x = 0;
            myTextBox[y, x] = textBox211; x++;
            myTextBox[y, x] = textBox212; x++;
            myTextBox[y, x] = textBox213; x++;
            myTextBox[y, x] = textBox214; x++;
            myTextBox[y, x] = textBox215; x++;
            myTextBox[y, x] = textBox216; x++;
            myTextBox[y, x] = textBox217; x++;
            myTextBox[y, x] = textBox218; x++;
            myTextBox[y, x] = textBox219; x++;
            myTextBox[y, x] = textBox220; x++;
            myTextBox[y, x] = textBox221; x++;
            myTextBox[y, x] = textBox222; x++;
            myTextBox[y, x] = textBox223; x++;
            myTextBox[y, x] = textBox224; x++;
            myTextBox[y, x] = textBox225; x++;
            y++; x = 0;
            for (int p = 0; p < 3; p++) 
            {
                int k = 0;
                while (k < 3)
                {
                    myTextBox[k, p].Visible = true;
                    k++;
                }
            }
                
        }

        private void UserInitialization()
        {
            _spManager = new SerialPortManager();
            SerialSettings mySerialSettings = _spManager.CurrentSerialSettings;
            serialSettingsBindingSource.DataSource = mySerialSettings;
           /* portNameComboBox.DataSource = mySerialSettings.PortNameCollection;
            baudRateComboBox.DataSource = mySerialSettings.BaudRateCollection;
            dataBitsComboBox.DataSource = mySerialSettings.DataBitsCollection;
            parityComboBox.DataSource = Enum.GetValues(typeof(System.IO.Ports.Parity));
            stopBitsComboBox.DataSource = Enum.GetValues(typeof(System.IO.Ports.StopBits));
            */
            _spManager.NewSerialDataRecieved += new EventHandler<SerialDataEventArgs>(_spManager_NewSerialDataRecieved);
            this.FormClosing += new FormClosingEventHandler(MainForm_FormClosing);
        }


        public static string sendbuffer = "";

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            _spManager.Dispose();
        }

        void _spManager_NewSerialDataRecieved(object sender, SerialDataEventArgs e)
        {
            if (sendbuffer.Length > 0)
            {
                _spManager.SerialSend(sendbuffer);
                sendbuffer = "";
            }

            if (this.InvokeRequired)
            {
                // Using this.Invoke causes deadlock when closing serial port, and BeginInvoke is good practice anyway.
                this.BeginInvoke(new EventHandler<SerialDataEventArgs>(_spManager_NewSerialDataRecieved), new object[] { sender, e });
                return;
            }
            // This application is connected to a GPS sending ASCCI characters, so data is converted to text
            string str = Encoding.ASCII.GetString(e.Data);
            parse_data(str);
        }


        public string bigstring = "";

        private void parse_data(string strr)
        {

            bigstring += strr;

            if(bigstring.IndexOf("@") != -1) 
            {
                bigstring = bigstring.Substring(bigstring.IndexOf("@"));
                if(bigstring.IndexOf(";") > bigstring.IndexOf("@") )
                {
                    _spManager.StopListening();
                    for (int x11 = 0; x11 < 15; x11++)
                    {
                        for (int x22 = 0; x22 < 15; x22++)
                        {
                            myTextBox[x11, x22].Text = "";
                            myTextBox[x11, x22].Visible = false;
                            myTextBox[x11, x22].BackColor = Color.Cornsilk;
                        }
                    }
                    MainForm.ActiveForm.Width = 325;
                    MainForm.ActiveForm.Height = 280;

                    string[] words = bigstring.Split(',');
                    int row_count = 0; int col_count = 0;
                    int part_count = 0;
       
                    foreach (string word in words)
                    {
                        if (word != null)
                        {
                            if (word.IndexOf('@') != -1)
                            {
                                row_count = 0;
                                col_count = 0;
                            }
                            else if (word.IndexOf('#') != -1)
                            {
                                row_count++;
                                col_count = 0;
                            }
                            else if (word.IndexOf('&') != -1)
                            {
                                part_count++;
                            }
                            else if (word.IndexOf(';') != -1)
                            {
                                break;
                            }
                            else if(part_count==0)
                            {
                                myTextBox[row_count, col_count].Text = word;
                                myTextBox[row_count, col_count].Visible = true;
                                col_count++;
                            }
                            else if(part_count==1)
                            {
                                sum_box.Text = word;
                            }
                            else if(part_count==2)
                            {
                                total_boxex_box.Text = word;
                            }
                        }
                    }

                    int biggest_value = 0;
                    int smallest_value = 65000;
                    int big_x = 0;
                    int big_y = 0;
                    int small_x = 0;
                    int small_y = 0;
                    for (int p = 0; p < 15; p++)
                    {
                        int k = 0;
                        while (k < 15)
                        {
                            if (myTextBox[k, p].Text.Length > 0)
                            {
                                int val = Convert.ToInt16(myTextBox[k, p].Text);

                                if (val >= biggest_value)
                                {
                                    biggest_value = val;
                                    big_x = k;
                                    big_y = p;
                                }
                                if (val <= smallest_value)
                                {
                                    smallest_value = val;
                                    small_x = k;
                                    small_y = p;
                                }
                            }
                            k++;
                        }
                    }

                    myTextBox[big_x, big_y].BackColor = Color.LightGreen;
                    myTextBox[small_x, small_y].BackColor = Color.LightBlue;

                    MainForm.ActiveForm.Width = MainForm.ActiveForm.Width + 10;
                    MainForm.ActiveForm.Height = MainForm.ActiveForm.Height + 10;
                    
                    bigstring = "";

                }
            }

        }

    

        private void MainForm_Load(object sender, EventArgs e)
        {

        }



        private void button_calcul_Click(object sender, EventArgs e)
        {
      //      try
      //      {
//                MessageBox.Show(_spManager.p)
                int order_input = Convert.ToInt32(order_box.Text);
                int remaind = order_input % 2;

                if ((order_input > 15) || (remaind==0))
                {
                    MessageBox.Show("ERROR: Please enter an Odd number between 3 and 15.");
                    return;
                }
                sendbuffer = "N=" + order_input + ";\r\n";
                _spManager.StartListening();
                //parse_data("@,11,12,13,14,15,#,21,22,23,24,25,#,31,32,33,34,35,#,41,42,43,44,45,#,51,52,53,54,55,#,&,66,&,90,;");
        //    }
        //    catch(Exception ex)
        //    {
        //        MessageBox.Show(ex.Message);
        //    }
        }

    }
}
