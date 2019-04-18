using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace _1.点名器
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label1.Text = "";
        }

        public static double[] dName = new double[0];


        private void button1_Click(object sender, EventArgs e)
        {
            string[] sName = File.ReadAllLines("名单.txt");

            Random rand = new Random();
            label1.Text = sName[rand.Next(0, sName.Length)];


        }
    }
}
