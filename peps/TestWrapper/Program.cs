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
        static void Main(string[] args)
        {
            DataAssetValuesLoader loader = new DataAssetValuesLoader(new DateTime(2004, 1, 1), new DateTime(2006, 1, 1));
            double[,] mat = loader.Matrix;
            DataAssetValuesLoader loaderNext = new DataAssetValuesLoader(new DateTime(2006, 2, 2), new DateTime(2014, 6, 6));
            double[,] matNext = loaderNext.Matrix;
            
            double[] vol;
            double[,] corr;
            
             WrapperClass cl = new WrapperClass();
             corr = cl.CalcCorr(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
             vol = cl.CalcVol(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
             cl.LaunchComputation(matNext, vol, corr, loaderNext.LstassetName.Count, loaderNext.LstassetDate.Count);
             Console.WriteLine("coucou");
        }

        
    }
}
