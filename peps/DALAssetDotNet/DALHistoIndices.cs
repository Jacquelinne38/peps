using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DALAssetDotNet
{
    public static class DALHistoIndices
    {

        //test function on cac
        public static List<ChartData> getCac()
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;

            List<ChartData> tmp = new List<ChartData>();

            var query = (from p in db.HistoIndices
                         orderby p.date
                         select p);
            
      
            foreach (var item in query)
            {
              
                tmp.Add(new ChartData(item.value, item.date));
            }
  
            return tmp;
        }

        public static List<String> GetIndicesList()
        {
            List<String> tmp = new List<string>();
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;

            //TODO

            return tmp;
        }




        public static double[] getValueInd(String nom, int WindowWidth, DateTime dateD, DateTime dateF)
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;

            double[] tmp = new double[WindowWidth];

            var query = (from action in db.HistoIndices
                         where action.name == nom && action.date >= dateD && action.date <= dateF
                         orderby action.date
                         select action.value);

            var tabquery = query.ToArray();

            //TODO bad compute case tabquery < windowsWidth
            for (int i = 0; (i < WindowWidth) && (i < tabquery.GetLength(0)); ++i)
            {
                tmp[i] = tabquery[i];
            }

            return tmp;
        }
        public static DateTime getDateD()
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            var query = (from action in db.HistoIndices
                         orderby action.date
                         select action.date).Min();
            return query;
        }

        public static DateTime getDateF(int windowWidth)
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            DateTime returnDate = new DateTime();

            var query = (from action in db.HistoIndices
                         orderby action.date
                         select action.date).Distinct();

            for (int i = 0; i < query.Count() && i < windowWidth; ++i)
            {
                returnDate = query.ToArray()[i];
            }

            return returnDate;
        }

        public static DateTime getNextDate(DateTime date, int WindowWidth, int delta)
        {
            //TODO check if we ve date at the end
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            DateTime returnDate = new DateTime();

            var query = (from action in db.HistoIndices
                         where action.date >= date
                         orderby action.date
                         select action.date).Distinct();
            for (int i = 0; i < query.Count() && i < delta; ++i)
            {
                returnDate = query.ToArray()[i];
            }
            return returnDate;
        }

    }

}
