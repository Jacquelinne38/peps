using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DALAssetDotNet
{
    public static class DALHistoComponents
    {
        private static List<HistoComponentsData> _histoData = new List<HistoComponentsData>();
        
        public static List<HistoComponentsData> getHistoComponents()
        {
            DataPepsDataContext db = new DataPepsDataContext();

            //speed up 
       /*     if (!(_histoData.Count > 0))
            {
                var query = (from p in db.HistoComponents
                             select p);

                foreach (var item in query)
                {

                    _histoData.Add(new HistoComponentsData(item.name, item.date, item.value));
                }
            }
            */
            return _histoData;
        }

        public static double[] getValueAsset(String nom, int windowWidth, DateTime dateD, DateTime dateF)
        {

            getHistoComponents();

            double[] tmp = new double[windowWidth];

            var query = (from action in _histoData
                         where action.Name == nom && action.Datetime >= dateD && action.Datetime <= dateF
                         orderby action.Datetime
                         select action.Value);

            var tabquery = query.ToArray();

            //TODO bad compute case tabquery < windowsWidth
            for (int i = 0; (i < windowWidth) && (i < tabquery.GetLength(0)); ++i)
            {
                tmp[i] = tabquery[i];
            }

            return tmp;
        }


        public static List<String> getDistinctAssetName()
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var query = (from p in db.HistoComponents
                         select p.name)
                                        .Distinct();

            foreach (var item in query)
            {
                tmp.Add(item.ToString());
            }

            return tmp;
        }


        public static List<DateTime> getListDateBetween(DateRebalancement dateRebalancement)
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            List<DateTime> returnDate = new List<DateTime>();

            var query = (from action in db.HistoComponents
                         where action.date >= dateRebalancement.DateBegin
                         && action.date <= dateRebalancement.DateEnd
                         orderby action.date
                         select action.date);
            foreach (var item in query)
            {
                returnDate.Add(item);
            }
            return returnDate;
        }


        public static double getValueAsset(string name, DateTime dateTime)
        {
            getHistoComponents();

            var query = (
                from p in _histoData
                where p.Datetime == dateTime && p.Name == name
                select p.Value);
                      
           
            return query.FirstOrDefault();
        }

        public static int getNbDate()
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            var query = (
                from p in db.HistoComponents
                select p.date).Distinct().Count();

            return query;
        }

        public static List<DateTime> getDate()
        {
            DataHistoDataContext db = DALAssetDotNet.ContextDb.Connexion;
            List<DateTime> returnDate = new List<DateTime>();

            var query = (from action in db.HistoComponents
                         where action.name == "AC.PA"
                         orderby action.date
                         select action.date);
            foreach (var item in query)
            {
                returnDate.Add(item);
            }
            return returnDate;
        }
    }
}
