using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Wrapper;
using DALAssetDotNet;


namespace TestWrapper
{
    class Program
    {
        /*
         * Projet pour tester le mapping du c# à c++/cli
         * Ce projet peu être lancé en mode .exe ou etre compilé comme une bibliothèque de classe.
         * C'est le projet d'entrée de notre projet asp.
         * Bien entendu le main nous sort exclusivement pour nos tests sans app web.
         * 
         */ 
        static void Main(string[] args)
        {
            DataAssetValuesLoader loader = new DataAssetValuesLoader(new DateTime(2004, 1, 1), new DateTime(2006, 1, 1));
            double[,] mat = loader.Matrix;
            DataAssetValuesLoader loaderNext = new DataAssetValuesLoader(new DateTime(2006, 2, 2), new DateTime(2014, 4, 4));
            double[,] matNext = loaderNext.Matrix;
            
            double[] vol;
            double[,] corr;

            List<double> price = new List<double>();
            List<double> priceCouverture = new List<double>();
            List<double> sansRisque = new List<double>();
            List<double> risque = new List<double>();
            
             WrapperClass cl = new WrapperClass();
             corr = cl.CalcCorr(mat, loader.LstassetName.Count, loader.LstassetDate.Count, true);
             vol = cl.CalcVol(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
             cl.LaunchComputation(matNext, 
                                  vol, 
                                  corr, 
                                  loaderNext.LstassetName.Count, 
                                  loaderNext.LstassetDate.Count,
                                  ref price,
                                  ref priceCouverture,
                                  ref sansRisque, 
                                  ref risque
                                  );

             DataResultEncoder.ExportData(loaderNext.LstassetDate, price, priceCouverture, sansRisque, risque);

             Console.WriteLine("Done");
        }


     

        
    }
}
