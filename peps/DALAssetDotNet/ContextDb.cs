using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DALAssetDotNet
{
    static class ContextDb
    {
    private static DataPepsDataContext _connexion = null;
    private static readonly object _locker = new object();

    public static DataPepsDataContext Connexion
    {
        get
        {
            //mutex thread-safe
            lock (_locker)
            {
                try
                {

                    _connexion = _connexion ?? new DataPepsDataContext();
                    //poor coding style but works
                    //if (_connexion == null || (_connexion.Connection.State != System.Data.ConnectionState.Open))
                    //{
                    //    throw new System.ArgumentException("Parameter cannot be null, no database connexion");
                    //}
                }
                catch (Exception e)
                {
                    Console.Out.WriteLine("Db connexion crash, exception : " + e.Message);
                }
                
                return _connexion;
            }
        }
    }
}
}
