using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
//using Wrapper;



/*
 * 
 * 
 * 
 * Ceci est un projet de débug que nous avons utilisé pour visualiser les résultats avant la mise en place des pages asp.net
 * Ce projet peu etre maintenant vu comme usless
 * 
 */ 
namespace GraphTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            List<double> lst_price = new List<double>();
            lst_price = LectureData(@"../../../DATA/prix.txt");
            List<double> lst_couverture = new List<double>();
            lst_couverture = LectureData(@"../../../DATA/couverture.txt");
            chart1.Series.Clear();
            chart1.Series.Add("Prix");
            //chart1.Series.Add("1.23");
            chart1.Series.Add("Couverture");
            // chart1.Series["Price"].AxisLabel = "Price";
            chart1.Series["Prix"].ChartType = SeriesChartType.Spline;
            //chart1.Series["1.23"].ChartType = SeriesChartType.Spline;
            chart1.Series["Couverture"].ChartType = SeriesChartType.Spline;
            for (int i = 0; i < lst_price.Count; i++)
            {
                chart1.Series["Prix"].Points.AddXY(i, lst_price[i]);

            }
            //for(int i = 0; i < 260; i++) {
            //    chart1.Series["1.23"].Points.AddXY(i, 1.23);
            //}
            for (int i = 0; i < lst_couverture.Count; ++i)
            {
                chart1.Series["Couverture"].Points.AddXY(i, lst_couverture[i]);
            }
             
 
           // WrapperClass cl = new WrapperClass();
           // cl.getPriceCallEuro();

        }

        private List<double> LectureData(string fichier) {
            List<double> lst_double = new List<double>();
            int counter = 0;
            string line;

            // Read the file and display it line by line.
            System.IO.StreamReader file = new System.IO.StreamReader(fichier);
            while ((line = file.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                line = line.Replace(".", ",");
                lst_double.Add(Convert.ToDouble(line));
                counter++;
            }

            file.Close();
            System.Console.WriteLine("There were {0} lines.", counter);
            // Suspend the screen.
            System.Console.ReadLine();
            return lst_double;
        }
    }
}
