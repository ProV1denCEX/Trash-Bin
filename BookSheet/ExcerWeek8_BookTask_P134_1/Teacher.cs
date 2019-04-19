using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExcerWeek8_BookTask_P134_1
{
    public class Teacher
    {
        private string name;
        private string title;

        public Teacher(string name, string title)
        {
            this.name = name;
            this.title = title;
        }

        public string OutoutMesg()
        {
            return string.Format("Name:{0}, title:{1}", this.name, this.title);
        }
    }
}
