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
            DataAssetValuesLoader loader = new DataAssetValuesLoader(new DateTime(2004, 1, 1), new DateTime(2005, 1, 1));
            double[,] mat = loader.Matrix;
            
             WrapperClass cl = new WrapperClass();
             cl.getPriceCallEuro(mat, loader.LstassetName.Count, loader.LstassetDate.Count);
        }
    }
}
