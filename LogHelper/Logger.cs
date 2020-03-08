using log4net;
using System.IO;

namespace LogHelper
{
    public class Logger
    {
        static public ILog Log { get; set; } = null;
        static public void InitLogger()
        {
            string strLog4netConfig = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "config/log4net.config");
            log4net.Config.XmlConfigurator.Configure(new FileInfo(strLog4netConfig));
            Log = LogManager.GetLogger("PFDLogger");
        }
    }
}
