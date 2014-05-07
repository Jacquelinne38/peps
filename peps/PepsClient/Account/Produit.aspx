<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Produit.aspx.cs" Inherits="PepsClient.Account.Produit" %>
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">
   <p>Le produit structuré est basé sous les sous-jacents suivant : </p>
    <asp:GridView ID="GridView1" runat="server" AutoGenerateColumns="False" 
    DataKeyNames="namego" DataSourceID="SqlDataSource1" 
    EmptyDataText="Il n'y a aucun enregistrement de données à afficher.">
    <Columns>
        <asp:BoundField DataField="namego" HeaderText="Google" ReadOnly="True" 
            SortExpression="namego" />
        <asp:BoundField DataField="nameyh" HeaderText="Yahoo" SortExpression="nameyh" />
        <asp:BoundField DataField="fullname" HeaderText="Name" 
            SortExpression="fullname" />
        <asp:BoundField DataField="sector" HeaderText="Sector" 
            SortExpression="sector" />
        <asp:BoundField DataField="country" HeaderText="Country" 
            SortExpression="country" />
        <asp:BoundField DataField="currency" HeaderText="Currency" 
            SortExpression="currency" />
    </Columns>
</asp:GridView>
<asp:SqlDataSource ID="SqlDataSource1" runat="server" 
    ConnectionString="<%$ ConnectionStrings:pepsConnectionString1 %>" 
    InsertCommand="INSERT INTO [AssetsInfo] ([namego], [nameyh], [fullname], [sector], [country], [currency]) VALUES (@namego, @nameyh, @fullname, @sector, @country, @currency)" 
    ProviderName="<%$ ConnectionStrings:pepsConnectionString1.ProviderName %>" 
    SelectCommand="SELECT [namego], [nameyh], [fullname], [sector], [country], [currency] FROM [peps].[AssetsInfo] order by [fullname]" 
    UpdateCommand="UPDATE [AssetsInfo] SET [nameyh] = @nameyh, [fullname] = @fullname, [sector] = @sector, [country] = @country, [currency] = @currency WHERE [namego] = @namego">
    <DeleteParameters>
        <asp:Parameter Name="namego" Type="String" />
    </DeleteParameters>
    <InsertParameters>
        <asp:Parameter Name="namego" Type="String" />
        <asp:Parameter Name="nameyh" Type="String" />
        <asp:Parameter Name="fullname" Type="String" />
        <asp:Parameter Name="sector" Type="String" />
        <asp:Parameter Name="country" Type="String" />
        <asp:Parameter Name="currency" Type="String" />
    </InsertParameters>
    <UpdateParameters>
        <asp:Parameter Name="nameyh" Type="String" />
        <asp:Parameter Name="fullname" Type="String" />
        <asp:Parameter Name="sector" Type="String" />
        <asp:Parameter Name="country" Type="String" />
        <asp:Parameter Name="currency" Type="String" />
        <asp:Parameter Name="namego" Type="String" />
    </UpdateParameters>
</asp:SqlDataSource>
   
</asp:Content>
