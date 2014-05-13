using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DataManagement;

namespace PepsClient.Account
{
    public partial class Compute_Parameter : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void ButtonCompute_Click(object sender, EventArgs e)
        {
            LabelEtat.Text = "In progress";
            DateTime begin = CalendarBegin.SelectedDate;
            DateTime end = CalendarEnd.SelectedDate;
            int tmp = 0;
            if(!(begin == null || end == null))
                tmp = TestWrapper.DataParameterEncoder.LaunchParameterComputation(begin, end);
            LabelEtat.Text = "Done with statut : "+tmp;

        }

    }
}