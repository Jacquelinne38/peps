using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DALAssetDotNet
{
    public static class DALParameter
    {
        //retourne les volatilité pour un id donnée
        public static double[] getVol(int idRun) {

            DataPepsDataContext db = ContextDb.Connexion;
            List<Volatility> vol = new List<Volatility>();
            vol = db.Volatility.Where(x => x.runId == idRun).ToList();

            double[] tmp = new double[vol.Count];
            for (int i = 0; i < vol.Count; ++i)
            {
                tmp[i] = (double)vol[i].vol;
            }
                return tmp;
        }
        //retourne la correlation pour un id donnée
        public static double[,] getCoor(int idRun) {
            DataPepsDataContext db = ContextDb.Connexion;
            List<Correlation> re = new List<Correlation>();
            re = db.Correlation.Where(x => x.runId == idRun).ToList();

            double[,] corr = new double[re.Count,re.Count];
            for (int i = 0; i < re.Count; ++i)
            {
                corr[i, 0] = (double)re.ToList()[i].AMS_DSM;
                corr[i, 1] = (double)re.ToList()[i].AMS_MT;
                corr[i, 2] = (double)re.ToList()[i].ETR_DB1;
                corr[i, 3] = (double)re.ToList()[i].ETR_IFX;
                corr[i, 4] = (double)re.ToList()[i].HEL_NOKIV;
                corr[i, 5] = (double)re.ToList()[i].LON_BARC;
                corr[i, 6] = (double)re.ToList()[i].NYSE_AA;
                corr[i, 7] = (double)re.ToList()[i].NYSE_CTL;
                corr[i, 8] = (double)re.ToList()[i].NYSE_DYN;
                corr[i, 9] = (double)re.ToList()[i].NYSE_GE;
                corr[i, 10] = (double)re.ToList()[i].NYSE_HOG;
                corr[i, 11] = (double)re.ToList()[i].NYSE_IP;
                corr[i, 12] = (double)re.ToList()[i].NYSE_JCP;
                corr[i, 13] = (double)re.ToList()[i].NYSE_MHFI;
                corr[i, 14] = (double)re.ToList()[i].NYSE_TIF;
                corr[i, 15] = (double)re.ToList()[i].TSE_BB;
                corr[i, 16] = (double)re.ToList()[i].HKG_0857;
                corr[i, 17] = (double)re.ToList()[i].TYO_7974;
                corr[i, 18] = (double)re.ToList()[i].TYO_8058;
                corr[i, 19] = (double)re.ToList()[i].TYO_4568;
            }
            return corr;


        }

        //renvoi le dernier id utilisé 
        public static int getLastRunVolId()
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.Volatility.Max(x => x.runId);
            if (result == null) result = 1;
            return result;
        }

        //renvoi la date de début du run
        public static string getDateBegin(int runId) {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.InfoRunId.Where(x => x.runId == runId).Single(); ;
            return result.dateBegin.ToString();
        }

        //renvoi la date du fin du run
        public static string getDateEnd(int runId)
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.InfoRunId.Where(x => x.runId == runId).Single();
            return result.dateEnd.ToString() ;
        }

        //renvoi le dernier id utilisé pour les info
        public static int getLastRunInfoId()
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.InfoRunId.Max(x => x.runId);
            if (result == null) result = 1;
            return result;
        }
        //renvoi le dernier id utilisé pour la correl
        public static int getLastRunCoorId()
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.Correlation.Max(x => x.runId);
            if (result == null) result = 1;
            return result;
        }
        //permet de sauvegarder des infos
        public static void SaveInfo(DateTime begin, DateTime end)
        {
            List<InfoRunId> results = new List<InfoRunId>();
            InfoRunId re = new InfoRunId();
            int id = getLastRunInfoId();
            re.runId =  (id+1);
            re.dateBegin = begin;
            re.dateEnd = end;
            results.Add(re);
            DataPepsDataContext db = ContextDb.Connexion;
            db.InfoRunId.InsertAllOnSubmit(results);
            db.SubmitChanges();
            //db.SubmitChanges();
        }

        //permet de sauvegarder la volatitilié
        public static void SaveVolResult(List<string> namego,
                                      List<double> vol
            )
        {
            int runid = getLastRunVolId();
            List<Volatility> results = new List<Volatility>();
            for (int i = 0; i < vol.Count; ++i)
            {
                Volatility re = new Volatility();
                re.runId = runid + 1;
                re.namego = namego[i];
                re.vol = vol[i];
                results.Add(re);
                // SaveOneResult(runid, dates[i], prices[i], priceCouvertures[i], sansRisques[i], risques[i]);
            }

            DataPepsDataContext db = ContextDb.Connexion;
            db.Volatility.InsertAllOnSubmit(results);
            db.SubmitChanges();
            //db.SubmitChanges();
        }

        //permet de sauvegarder la corrélation
        public static void SaveCorrResult(List<string> namego,
                                     double[,] corr
           )
        {
            int runid = getLastRunCoorId();
            List<Correlation> results = new List<Correlation>();
            for (int i = 0; i < namego.Count; ++i)
            {
                Correlation re = new Correlation();
                re.runId = runid + 1;
                re.AMS_DSM = corr[i,0];
                re.AMS_MT =  corr[i,1];
                re.ETR_DB1 = corr[i,2];
                re.ETR_IFX = corr[i,3];
                re.HEL_NOKIV=corr[i,4];
                re.LON_BARC= corr[i,5];
                re.NYSE_AA = corr[i,6];
                re.NYSE_CTL =corr[i,7];
                re.NYSE_DYN = corr[i,8];
                re.NYSE_GE = corr[i,9];
                re.NYSE_HOG= corr[i,10];
                re.NYSE_IP = corr[i,11];
                re.NYSE_JCP= corr[i,12];
                re.NYSE_MHFI=corr[i,13];
                re.NYSE_TIF= corr[i,14];
                re.TSE_BB=   corr[i,15];
                re.HKG_0857 =corr[i,16];
                re.TYO_7974= corr[i,17];
                re.TYO_8058= corr[i,18];
                re.TYO_4568 =corr[i,19];
                results.Add(re);
            }

            DataPepsDataContext db = ContextDb.Connexion;
            db.Correlation.InsertAllOnSubmit(results);
            db.SubmitChanges();
        }
    }
}
