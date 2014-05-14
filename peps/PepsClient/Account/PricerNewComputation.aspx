<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="PricerNewComputation.aspx.cs" Inherits="PepsClient.Account.PricerNewComputation" %>
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <div style="border-top-style=solid;border-width:5px;"></div>
    <h2>Launch a new computation</h2>
<p>Correlation and volatility run :
     <asp:DropDownList ID="DropDownListVolCor" runat="server" AutoPostBack="True" 
        DataSourceID="SqlDataSource3" DataTextField="runId" DataValueField="runId">
        </asp:DropDownList>
    
    <asp:SqlDataSource ID="SqlDataSource3" runat="server" 
        ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
        SelectCommand="SELECT [runId] FROM [InfoRunId] where [runId] > 0 ORDER BY [runId] DESC">
    </asp:SqlDataSource>
    
 </p>
 <p>Discretisation type
     :

     <asp:RadioButtonList ID="RadioButtonListWeekDay" runat="server">
         <asp:ListItem Value="0">Week</asp:ListItem>
         <asp:ListItem Value="1">Day</asp:ListItem>
     </asp:RadioButtonList>
  </p>
<p> Number of path : 
    <asp:TextBox ID="TextBoxPath" runat="server" AutoPostBack="True">100</asp:TextBox>
    </p>
    <div style="width : 200px; height:300px; float: left; margin-right : 100px;">
    <p> Begin date :
        <asp:Calendar ID="CalendarBegin" runat="server" VisibleDate="02/02/2006 00:00:00"></asp:Calendar>
    </p>
    </div>
     <p> End date :
        <asp:Calendar ID="CalendarEnd" runat="server"></asp:Calendar>
    </p>
    <asp:Button ID="ButtonLaunch" runat="server" Text="Run" 
        onclick="ButtonLaunch_Click" />
    <asp:Label ID="LabelInfo" runat="server"></asp:Label>
</asp:Content>
