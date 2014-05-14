<%@ Page Title="Qui sommes-nous" Language="C#" MasterPageFile="~/Site.master" AutoEventWireup="true"
    CodeBehind="About.aspx.cs" Inherits="PepsClient.About" %>

<asp:Content ID="HeaderContent" runat="server" ContentPlaceHolderID="HeadContent">
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js" type="text/javascript"></script>
<script type="text/JavaScript" src="/Scripts/jquery.cycle.js"></script>
<script type="text/JavaScript" src="/Scripts/cloud-carousel.1.0.3.js"></script>


<script>
    $(document).ready(function () {

        // This initialises carousels on the container elements specified, in this case, carousel1.
        $("#carousel1").CloudCarousel(
					{
					    xPos: 292, // coord x du center du cercle
					    yPos: 65, // coord y du centre du cercle
					    //xRadius: 292, // largeur sur laquelle les elements tournent (en px)
					    yRadius: 125, // largeur sur laquelle les elements tournent (en px)
					    buttonLeft: $("#left-but"),
					    buttonRight: $("#right-but"),
					    altBox: $("#alt-text"),
					    titleBox: $("#title-text"),
					    speed: 0.075
					    //autoRotate: 'left',
					    //autoRotateDelay: 3000
					}
				);
    });

    $(document).ready(function () {
        $('#diapo').cycle({
            fx: 'fade',
            timeout: 4000,
            next: '#next2',
            prev: '#prev2',
            random: true
        });
    });
 
</script>

</asp:Content>
<asp:Content ID="BodyContent" runat="server" ContentPlaceHolderID="MainContent">
<div id="left" style="width:584px; float:left;">
    <h2>
        L'équipe :
    </h2>
    <div id="carousel1"  style="width:584px; height:400px;  overflow:scroll;">
		    <img  width="130" height="160" class = "cloudcarousel" src="./img/yannick.jpg" alt="Option Ingénierie de l'Information et Mathématiques Financières,<br /><span style='color:#1860A8'>yannick.luzy[at]gmail.com</span>" title="Yannick Luzy" />
			<img  width="250" height="200" class = "cloudcarousel" src="./img/lutratt.jpg" alt="Option Méthodes Quantitatives Avancées<br /><span style='color:#1860A8'>Thomas.Lutrat[at]gmail.com</span>" title="Thomas Lutrat" />
			<img  width="200" height="160" class = "cloudcarousel" src="./img/bourillp.jpg" alt="Option Méthodes Quantitatives Avancées<br /><span style='color:#1860A8'>Pierre.Bourillet[at]gmail.com</span>" title="Pierre Bourillet" />									
	</div>
    <table style="width:100%;">
        <tr>
	        <td><input id="left-but" type="button" value="<<" /></td>
	        <td>
		        <p id="title-text" style="text-align:center; margin:0; font-weight:bold; font-size:1.1em; text-indent:0;"></p>
		        <p id="alt-text" style="text-align:center; margin:0; text-indent:0;"></p>
	        </td>
	        <td style="text-align:right;">
		        <input id="right-but" type="button" value=">>" /></td>
        </tr>
    </table>
</div>
<div id ="right">
<p>Notre équipe est composée de trois personne :</p>
<ul style="margin-left:580px; margin-top:0px; padding-top:0px">
<li>Yannick Luzy :</br>Option Ingénierie de l'Information et Mathématiques Financières</li>
<li>Pierre Bourillet :</br>Option Méthodes Quantitatives Avancées</li>
<li>Thomas Lutrat :</br>Option Méthodes Quantitatives Avancées</li>
</ul>
</div>
</asp:Content>
