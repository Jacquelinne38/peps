<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Pricer.aspx.cs" Inherits="PepsClient.Account.MembersOnly" %>
<%@ Register assembly="System.Web.DataVisualization, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" namespace="System.Web.UI.DataVisualization.Charting" tagprefix="asp" %>
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <asp:Chart ID="Chart1" runat="server" DataSourceID="SqlDataSource1" 
        Width="919px">
    <series>
        <asp:Series ChartType="Spline" Name="Price" XValueMember="date" 
            YValueMembers="price">
        </asp:Series>
         <asp:Series ChartType="Spline" Name="Couverture" XValueMember="date" 
            YValueMembers="priceCouverture">
        </asp:Series>
    </series>
    <chartareas>
        <asp:ChartArea Name="ChartArea1">
        </asp:ChartArea>
    </chartareas>
</asp:Chart>
    <asp:SqlDataSource ID="SqlDataSource1" runat="server" 
        ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
        SelectCommand="SELECT [date], [price], [priceCouverture] FROM [Result] where [idRun] = @runId ORDER BY [date] ">
        
          <SelectParameters>
            <asp:Parameter Name="runId" Type="Int32" DefaultValue="1" />
          </SelectParameters>
    </asp:SqlDataSource>
    <p>Select your run : <asp:DropDownList ID="DropDownList1" runat="server" 
        DataSourceID="SqlDataSource2" DataTextField="idRun" DataValueField="idRun" 
        AutoPostBack="True" onselectedindexchanged="DropDownList1_SelectedIndexChanged">
    </asp:DropDownList>
    </p> 
    <asp:SqlDataSource ID="SqlDataSource2" runat="server" 
        ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
        SelectCommand="SELECT DISTINCT [idRun] FROM [Result] WHERE [idRun] > 0 ORDER BY [idRun] desc">
    </asp:SqlDataSource>
  
</asp:Content>
