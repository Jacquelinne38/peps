<%@ Page Title="Coorélation et volatilité" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="CalcParameter.aspx.cs" Inherits="PepsClient.Account.CalcParameter" %>
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">
    <p>Please select your volatility and correlation run : </p>
    <asp:DropDownList ID="DropDownListIdCalc" runat="server"
            DataSourceID="SqlDataSource3" DataTextField="runId" DataValueField="runId" 
            onselectedindexchanged="DropDownListIdCalc_SelectedIndexChanged" 
            AutoPostBack="True">
    </asp:DropDownList>


        <asp:SqlDataSource ID="SqlDataSource3" runat="server" 
            ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
            SelectCommand="SELECT [runId] FROM [InfoRunId] where [runId] > 0 ORDER BY [runId] DESC ">
        </asp:SqlDataSource>
        <asp:SqlDataSource ID="SqlDataSource1" runat="server" 
            ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
             SelectCommand="SELECT [vol], [namego] FROM [Volatility] where [runId] = @runId">

              
          <SelectParameters>
            <asp:Parameter Name="runId" Type="Int32" DefaultValue="0" />
          </SelectParameters>
        </asp:SqlDataSource>
   
<h1>Les paramètres suivant on été calculer avec : </h1>
    <p> Begin date : 
    <asp:Label ID="LabelDateDebut" runat="server"></asp:Label>
    </p>
    <p> End date&nbsp;&nbsp;&nbsp; :    
    <asp:Label ID="LabelDateFin" runat="server"></asp:Label>
    </p>
    <p> Note (please modify) :
        <asp:TextBox ID="TextBoxNote" runat="server" Height="84px" Width="699px"></asp:TextBox>
    </p>
    <p> Correlation matrix : 
    <asp:GridView ID="GridView1" runat="server" AutoGenerateColumns="False" 
            DataSourceID="SqlDataSource2" CellPadding="4" ForeColor="#333333" 
            GridLines="None">
        <AlternatingRowStyle BackColor="White" />
        <Columns>
            <asp:BoundField DataField="column1" HeaderText="AMS:DSM" 
                SortExpression="column1" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column2" HeaderText="AMS:MT" 
                SortExpression="column2" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column3" HeaderText="ETR:DB1" 
                SortExpression="column3" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column4" HeaderText="ETR:IFX" 
                SortExpression="column4" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column5" HeaderText="HEL:NOKIV" 
                SortExpression="column5" DataFormatString="{0:0.0000}" />
           
            <asp:BoundField DataField="column7" HeaderText="LON:BARC" 
                SortExpression="column7" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column8" HeaderText="NYSE:AA" 
                SortExpression="column8" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column9" HeaderText="NYSE:CTL" 
                SortExpression="column9" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column10" HeaderText="NYSE:DYN" 
                SortExpression="column10" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column11" HeaderText="NYSE:GE" 
                SortExpression="column11" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column12" HeaderText="NYSE:HOG" 
                SortExpression="column12" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column13" HeaderText="NYSE:IP" 
                SortExpression="column13" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column14" HeaderText="NYSE:JCP" 
                SortExpression="column14" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column15" HeaderText="NYSE:MHFI" 
                SortExpression="column15" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column16" HeaderText="NYSE:TIF" 
                SortExpression="column16" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column17" HeaderText="TSE:BB" 
                SortExpression="column17" DataFormatString="{0:0.0000}" />
                 <asp:BoundField DataField="column6" HeaderText="HKG:0857" 
                SortExpression="column6" DataFormatString="{0:0.0000}" />
                            <asp:BoundField DataField="column20" HeaderText="TYO:7974" 
                SortExpression="column20" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column19" HeaderText="TYO:8058" 
                SortExpression="column18" DataFormatString="{0:0.0000}" />
            <asp:BoundField DataField="column18" HeaderText="TYO:4568" 
                SortExpression="column19" DataFormatString="{0:0.0000}" />

        </Columns>
        <EditRowStyle BackColor="#2461BF" />
        <FooterStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <HeaderStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <PagerStyle BackColor="#2461BF" ForeColor="White" HorizontalAlign="Center" />
        <RowStyle BackColor="#EFF3FB" />
        <SelectedRowStyle BackColor="#D1DDF1" Font-Bold="True" ForeColor="#333333" />
        <SortedAscendingCellStyle BackColor="#F5F7FB" />
        <SortedAscendingHeaderStyle BackColor="#6D95E1" />
        <SortedDescendingCellStyle BackColor="#E9EBEF" />
        <SortedDescendingHeaderStyle BackColor="#4870BE" />
    </asp:GridView>
        <asp:SqlDataSource ID="SqlDataSource2" runat="server" 
            ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
            SelectCommand="SELECT [AMS:DSM] AS column1, [AMS:MT] AS column2, [ETR:DB1] AS column3, [ETR:IFX] AS column4, [HEL:NOKIV] AS column5, 
            [HKG:0857] AS column6, [LON:BARC] AS column7, [NYSE:AA] AS column8, [NYSE:CTL] AS column9, 
            [NYSE:DYN] AS column10, [NYSE:GE] AS column11, [NYSE:HOG] AS column12, [NYSE:IP] AS column13, 
            [NYSE:JCP] AS column14, [NYSE:MHFI] AS column15, [NYSE:TIF] AS column16, [TSE:BB] AS column17, 
            [TYO:4568] AS column18, [TYO:8058] AS column19, [TYO:7974] AS column20 FROM [Correlation]
            where [runId] = @runId">
         
          <SelectParameters>
            <asp:Parameter Name="runId" Type="Int32" DefaultValue="0" />
          </SelectParameters>
        
        </asp:SqlDataSource>
    </p>
    <p> Volatility vector : 
    <asp:GridView ID="GridView2" runat="server" AutoGenerateColumns="False" 
            DataSourceID="SqlDataSource1">
        <Columns>
            <asp:BoundField DataField="vol" HeaderText="vol" SortExpression="vol" />
            <asp:BoundField DataField="namego" HeaderText="namego" 
                SortExpression="namego" />
        </Columns>
    </asp:GridView>
    </p>

    </asp:Content>
