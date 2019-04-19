using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.IO;

namespace ExcerWeek8_BookTask_P134_1
{
    public partial class Form1 : Form
    {
        ArrayList TeacherList = new ArrayList();
        public Form1()
        {
            InitializeComponent();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            Teacher newTeacher = new Teacher(textBoxName.Text, textBoxTitle.Text);
            TeacherList.Add(newTeacher);
            StreamWriter testText = new StreamWriter(@"E:\C#学习\ExcerWeek8_BookTask_P134_1\TeextTest.txt", true);
            foreach (object objTeacher in TeacherList)
            {
                Teacher newTeach = (Teacher)objTeacher;
                listBoxResult.Items.Add("\n" + newTeach.OutoutMesg());
                testText.WriteLine("\n" + newTeach.OutoutMesg());
            }
            testText.Close();
        }
    }
}
