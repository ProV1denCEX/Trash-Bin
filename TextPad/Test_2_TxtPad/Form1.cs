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


namespace Test_2_TxtPad
{
    public partial class TxtPad : Form
    {
        public TxtPad()
        {
            InitializeComponent();
            /// 设置滚轮
            textBoxContent.ScrollBars = RichTextBoxScrollBars.ForcedVertical;
        }
        /// <summary>
        /// 初始状态下文件名和路径名
        /// </summary>
        private string filename = "";
        private string filepath = "";
        private string selectedinfo = "";

        public FindForm findform;
        /// <summary>
        /// 新建文件时
        ///     1、考虑是否有文本，有的话要先保存
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 新建ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /// 先文件名初始化
            filename = "";
            if (textBoxContent.Text != string.Empty)
            {
                if(MessageBox.Show("Save it or Not", "Attention", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
                {
                    /// 保存动作： 保存 改程序名 清空输入板
                    Save();
                    this.Text = filename + " Frontal's NotePad";
                    textBoxContent.Clear();
                }
                else
                {
                    /// 不保存动作： 改程序名 清空输入板
                    this.Text = filename + " Frontal's NotePad";
                    textBoxContent.Clear();
                }
            }
        }

        /// <summary>
        /// 保存动作
        ///     1、是否是第一次保存：看是否有文件名
        ///     2、是 则 另存为
        ///     3、不是 则 覆盖内容
        /// </summary>
        private void Save()
        {
            if (!filename.Equals(""))
            {
                SaveFileDialog savefile = new SaveFileDialog();
                savefile.Filter = "text(*.txt)|*txt";

                StreamWriter sw = new StreamWriter(filepath, false);
                sw.Write(textBoxContent.Text);
                sw.Close();

                MessageBox.Show("File Saved !", "", MessageBoxButtons.OK, MessageBoxIcon.None);

                this.Text = this.filename + " Frontal's NotePad"; 
            }
            else
            {
                SaveOther();
            }
        }

        /// <summary>
        ///  另存为动作
        ///  存储完成后改文件名和程序名
        /// </summary>
        private void SaveOther()
        {
            SaveFileDialog savefile = new SaveFileDialog();
            savefile.Filter = "text(*.txt)|*txt";
            savefile.FileName = "NewFile.txt";
            if (savefile.ShowDialog() == DialogResult.OK)
            {
                StreamWriter sw = new StreamWriter(savefile.FileName, false);
                sw.Write(textBoxContent.Text);
                sw.Close();
                MessageBox.Show("File Saved !", "", MessageBoxButtons.OK, MessageBoxIcon.None);
                this.filepath = savefile.FileName;
            }
            else
            {
                savefile.FileName = "";
                return;
            }

            if (filename.Equals(""))
            {
                FileInfo fileinfo = new FileInfo(savefile.FileName);
                this.filename = fileinfo.Name;
                this.Text = this.filename + " Frontal's NotePad"; 
            }
            else
            {
                this.Text = this.filename + " Frontal's NotePad"; 
            }
            
        }

        private void 保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Save();
        }

        private void 另存为ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveOther();
        }

        /// <summary>
        /// 退出动作
        /// 退出前会提示是否保存
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Save it or Not", "", MessageBoxButtons.YesNo, MessageBoxIcon.None);
            if(dr == DialogResult.Yes)
            {
                Save();
            }
            Application.Exit();
        }

        /// <summary>
        /// 打开动作
        /// 打开前会要求是否保存
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 打开ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /// 保存动作
            DialogResult dr = MessageBox.Show("Save it or Not", "", MessageBoxButtons.YesNo, MessageBoxIcon.None);
            if (dr == DialogResult.Yes)
            {
                Save();
            }

            /// 打开动作
            OpenFileDialog openfile = new OpenFileDialog();
            openfile.Filter = "text(*.txt)|*txt";
            if (openfile.ShowDialog() == DialogResult.OK)
            {
                StreamReader sr = new StreamReader(openfile.FileName);
                textBoxContent.Text = sr.ReadToEnd();
                sr.Close();

                FileInfo fileinfo = new FileInfo(openfile.FileName);
                this.Text = fileinfo.Name + "Frontal's NotePad";
                filename = fileinfo.Name;

            }
        }

        /// <summary>
        /// 进行编辑动作
        /// 编辑前需要判断是否存在文本以进行编辑
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 编辑ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (textBoxContent.SelectedText.Equals("") && selectedinfo.Equals(""))
            {
                复制ToolStripMenuItem.Enabled = false;
            }
            else
            {
                复制ToolStripMenuItem.Enabled = true;
            }
        }

        private void 复制ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.textBoxContent.Copy();
        }

        private void 粘贴ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.textBoxContent.Paste();
        }

        /// <summary>
        /// 调整颜色
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 颜色ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog colorDig = new ColorDialog();
            colorDig.ShowDialog();
            textBoxContent.SelectionColor = colorDig.Color;
        }

        /// <summary>
        /// 调整字体
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 字体ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FontDialog fontDig = new FontDialog();
            fontDig.ShowDialog();
            textBoxContent.SelectionFont = fontDig.Font;
        }

        private void 查找ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            findform = new FindForm();
            findform.Show();
        }

        /// <summary>
        /// 加入快捷键
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void textBoxContent_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.S && e.Control)
            {
                e.Handled = true;  //将Handled设置为true，指示已经处理过KeyPress事件 
                Save();
            }

            if (e.KeyCode == Keys.O && e.Control)
            {
                打开ToolStripMenuItem_Click(this, e);
            }

            if (e.KeyCode == Keys.N && e.Control)
            {
                新建ToolStripMenuItem_Click(sender, e);
            }

            if (e.KeyCode == Keys.P && e.Control)
            {
                SaveOther();
            }

            if (e.KeyCode == Keys.F && e.Control)
            {
                查找ToolStripMenuItem_Click(sender, e);
            }
        }

    }
        
}
