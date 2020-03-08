using log4net;
using System.IO;

namespace LogHelper
{
    public class Logger
    {
        static public ILog WPFLog { get; set; } = null;
        static public void InitLogger()
        {
            string strLog4netConfig = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "config/WPFLog.config");
            log4net.Config.XmlConfigurator.Configure(new FileInfo(strLog4netConfig));
            WPFLog = LogManager.GetLogger("PFDLogger");
        }
    }
}
