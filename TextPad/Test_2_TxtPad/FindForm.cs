using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Test_2_TxtPad
{
    public partial class FindForm : Form
    {
        private string FindWord { get; set; }
        private string sSearch = "";

        public FindForm()
        {
            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterParent;
            btnSearch.Enabled = false;
            textBoxSearch.Text = sSearch;
        }

        /// <summary>
        /// 搜索模块
        /// </summary>
        private void find()
        {
            int pos;
            if (sSearch.Length == 0)
            {
                return;
            }

            pos = textBoxSearch.SelectionStart - 1;
            if (pos >= 0)
            {
                pos = textBoxSearch.Text.LastIndexOf(sSearch, pos);
                textBoxSearch.Select(pos, sSearch.Length);
            }
            else
                NoFound();

        }

        /// <summary>
        /// 无法找到结果
        /// </summary>
        private void NoFound()
        {
            MessageBox.Show("Cannot Find Result", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            sSearch = textBoxSearch.Text;
            find();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// 搜索字符的动态启用
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void textBoxSearch_TextChanged(object sender, EventArgs e)
        {
            if (textBoxSearch.Text.Length == 0)
            {
                btnSearch.Enabled = false;
            }
            else
            {
                btnSearch.Enabled = true;
            }
        }

    }
}
