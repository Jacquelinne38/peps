using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Wrapper;

namespace TestWrapper
{
    public static  class LaunchRun
    {
        //Fonction appelé par le projet asp pour lancer un nouveau run de calcul.
        //Il prend en compte les divers paramètres qui sont renseignés sur la page de lancement de run.
        static public void LaunchRunPricer(int runVol, int weekDay, int nbPath, DateTime begin, DateTime end) {
            //DataAssetValuesLoader loader = new DataAssetValuesLoader(new DateTime(2004, 1, 1), new DateTime(2006, 1, 1));
            //double[,] mat = loader.Matrix;
            DataAssetValuesLoader loaderNext = new DataAssetValuesLoader(begin, end);
            double[,] matNext = loaderNext.Matrix;

            double[] vol;
            double[,] corr;
            double[,] compoAll = new double[1,1];

            List<double> price = new List<double>();
            List<double> priceCouverture = new List<double>();
            List<double> sansRisque = new List<double>();
            List<double> risque = new List<double>();

            WrapperClass cl = new WrapperClass();
           // corr = cl.CalcCorr(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
            //vol = cl.CalcVol(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
            corr = DALAssetDotNet.DALParameter.getCoor(runVol);
            vol = DALAssetDotNet.DALParameter.getVol(runVol);
            cl.LaunchComputation(matNext,
                                 vol,
                                 corr,
                                 loaderNext.LstassetName.Count,
                                 loaderNext.LstassetDate.Count,
                                 ref price,
                                 ref priceCouverture,
                                 ref sansRisque,
                                 ref risque,
                                 ref compoAll,
                                 weekDay,
                                 nbPath
                                 );

            DataResultEncoder.ExportData(loaderNext.LstassetDate, price, priceCouverture, sansRisque, risque, compoAll, loaderNext.LstassetName);

            Console.WriteLine("Done");

        }
    }
}
