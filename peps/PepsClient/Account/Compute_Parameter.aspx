<%@ Page Title="Compute parameter" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Compute_Parameter.aspx.cs" Inherits="PepsClient.Account.Compute_Parameter" %>
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">
<h1>Here you can launch a new computation of volatility and correlation parameters</h1>
<div style="width : 200px; height:300px; float: left; margin-right : 100px;">
<p> Begin date :
    <asp:Calendar ID="CalendarBegin" runat="server" VisibleDate="2004-01-01"></asp:Calendar>
</p>
</div>
<div>
<p> End date :
    <asp:Calendar ID="CalendarEnd" runat="server" VisibleDate="2006-01-01"></asp:Calendar>
</p>
</div style="margin-top:-50px;">

    <asp:Button ID="ButtonCompute" runat="server" Text="Compute" 
        onclick="ButtonCompute_Click" style="height: 26px" />
    <asp:Label ID="LabelEtat" runat="server"></asp:Label>
</asp:Content>
