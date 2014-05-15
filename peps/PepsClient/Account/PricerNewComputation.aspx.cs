using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace PepsClient.Account
{
    public partial class PricerNewComputation : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void ButtonLaunch_Click(object sender, EventArgs e)
        {
            LabelInfo.Text = "Launched";
            try
            {
                int weekDay = int.Parse(RadioButtonListWeekDay.SelectedItem.Value);
                int nbPath = int.Parse(TextBoxPath.Text);
                DateTime begin = CalendarBegin.SelectedDate;
                DateTime beginBis = new DateTime(2007, 1, 1);
                int compare = DateTime.Compare(begin, beginBis);
                if (compare > 0)
                    begin = beginBis;
                DateTime end = CalendarEnd.SelectedDate;
                int runId = int.Parse(DropDownListVolCor.SelectedValue);
                TestWrapper.LaunchRun.LaunchRunPricer(runId, weekDay, nbPath, begin, end);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Erreur parameter" + ex.Message);
                LabelInfo.Text = "Erreur";
            }
            LabelInfo.Text = "Done";
        }
    }
}