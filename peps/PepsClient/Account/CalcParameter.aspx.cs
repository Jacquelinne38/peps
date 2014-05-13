using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace PepsClient.Account
{
    public partial class CalcParameter : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void DropDownListIdCalc_SelectedIndexChanged(object sender, EventArgs e)
        {
            SqlDataSource1.SelectParameters["runId"].DefaultValue = DropDownListIdCalc.SelectedValue;
            SqlDataSource2.SelectParameters["runId"].DefaultValue = DropDownListIdCalc.SelectedValue;
            LabelDateDebut.Text = DALAssetDotNet.DALParameter.getDateBegin(int.Parse(DropDownListIdCalc.SelectedValue));
            LabelDateFin.Text = DALAssetDotNet.DALParameter.getDateEnd(int.Parse(DropDownListIdCalc.SelectedValue));
            
        }
    }
}