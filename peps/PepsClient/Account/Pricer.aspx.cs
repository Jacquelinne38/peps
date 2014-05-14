using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace PepsClient.Account
{
    public partial class MembersOnly : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            SqlDataSource1.SelectParameters["runId"].DefaultValue = DropDownList1.SelectedValue;
        }

        protected void DropDownList1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SqlDataSource1.SelectParameters["runId"].DefaultValue =  DropDownList1.SelectedValue;
            SqlDataSource3.SelectParameters["runId"].DefaultValue = DropDownList1.SelectedValue;
            SqlDataSource4.SelectParameters["runId"].DefaultValue = DropDownList1.SelectedValue;
            SqlDataSource4.SelectParameters["dateId"].DefaultValue = DropDownList2.SelectedValue;
        }

        protected void DropDownList2_SelectedIndexChanged(object sender, EventArgs e)
        {
            SqlDataSource4.SelectParameters["runId"].DefaultValue = DropDownList1.SelectedValue;
            SqlDataSource4.SelectParameters["dateId"].DefaultValue = DropDownList2.SelectedValue;
        }

    }
}