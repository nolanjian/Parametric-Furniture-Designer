using log4net;
using System;
using System.Windows;
using System.Windows.Forms;

namespace WPFGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private CLROSG.Wrapper Wrapper { get; set; } = null;
        private PictureBox DrawingArea { get; set; } = null;
        public static ILog Log { get; set; } = LogHelper.Logger.WPFLog;

        public MainWindow()
        {
            LogHelper.Logger.InitLogger();
            Log = LogHelper.Logger.WPFLog;
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
            if (Wrapper == null || Wrapper.Render(DrawingArea.Handle) == false)
            {
                Log.Error("Init Render Fail!");
            }
        }
    }
}
