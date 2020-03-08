using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPFGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private CLROSG.Wrapper Wrapper { get; set; } = null;
        private PictureBox DrawingArea { get; set; } = null;
        public static ILog Log { get; set; } = LogHelper.Logger.Log;

        public MainWindow()
        {
            LogHelper.Logger.InitLogger();
            Log = LogHelper.Logger.Log;
            Log.Info("PFD Start");
            

            InitializeComponent();
            Wrapper = new CLROSG.Wrapper();
            DrawingArea = new PictureBox();
            windowsFormHost.Child = DrawingArea;
            DrawingArea.Paint += new PaintEventHandler(DrawingArea_Paint);
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            if (Wrapper != null)
            {
                Wrapper.Destroy();
            }

            Log.Info("PFD End");
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {

        }

        private void DrawingArea_Paint(object sender, PaintEventArgs e)
        {
            if (Wrapper != null)
            {
                Wrapper.Render(DrawingArea.Handle);
            }
        }
    }
}
