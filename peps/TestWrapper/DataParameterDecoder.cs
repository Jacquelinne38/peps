using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestWrapper
{
    public static class DataParameterDecoder
    {
        private static double[] ImportVolData(int idRun)
        {
            return DALAssetDotNet.DALParameter.getVol(idRun);
        }
        private static double[,] ImportCorrData(int idRun)
        {
            return DALAssetDotNet.DALParameter.getCoor(idRun);
        }
    }
}
