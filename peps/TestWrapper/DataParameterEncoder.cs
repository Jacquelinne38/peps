using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DALAssetDotNet;
using Wrapper;


namespace TestWrapper
{
    public static class DataParameterEncoder
    {

        //Fonction d'entrée du projet web, qui permet de calculer les nouvelles corrélation et volatilité
        public static int LaunchParameterComputation(DateTime begin, DateTime end)
        {
            DataAssetValuesLoader loader = new DataAssetValuesLoader(begin, end);
            double[,] mat = loader.Matrix;

            double[] vol;
            double[,] corr;

            WrapperClass cl = new WrapperClass();
            corr = cl.CalcCorr(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);
            vol = cl.CalcVol(mat, loader.LstassetName.Count, loader.LstassetDate.Count, false);

            List<double> lst_vol = new List<double>();
            for (int i = 0; i < loader.LstassetName.Count; ++i)
            {
                lst_vol.Add(vol[i]);
            }


            DataParameterEncoder.ExportVolData(lst_vol, loader.LstassetName);
            DataParameterEncoder.ExportCorrData(corr, loader.LstassetName);
            DataParameterEncoder.ExportInfo(loader.Begin, loader.End);
            return 1;

        }

        private static void ExportVolData(List<double> vol, List<String> lst_name)
        {
            DALAssetDotNet.DALParameter.SaveVolResult(lst_name, vol);
        }
        private static void ExportCorrData(double[,] coor, List<String> lst_name)
        {
            DALAssetDotNet.DALParameter.SaveCorrResult(lst_name, coor);
        }
        private static void ExportInfo(DateTime begin, DateTime end)
        {
            DALAssetDotNet.DALParameter.SaveInfo(begin, end);
        }
    }
}
