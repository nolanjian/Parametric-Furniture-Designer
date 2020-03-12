using log4net;
using MaterialDesignThemes.Wpf;
using System;
using System.Windows;
using System.Windows.Forms;
using WPFGUI.ViewModel;

namespace WPFGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private PictureBox DrawingArea { get; set; } = null;
        public static ILog Log { get; set; } = LogHelper.Logger.WPFLog;

        public MainWindow()
        {
            LogHelper.Logger.InitLogger();
            Log = LogHelper.Logger.WPFLog;
            Log.Info("PFD Start");
            

            InitializeComponent();
            
            DrawingArea = new PictureBox();
            windowsFormHost.Child = DrawingArea;
            DrawingArea.Paint += new PaintEventHandler(DrawingArea_Paint);

            

            //DataContext = new MainWindowViewModel();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            CLRResource.OSGAdapt.Wrapper.Destroy();
            Log.Info("PFD End");
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {

        }

        private void DrawingArea_Paint(object sender, PaintEventArgs e)
        {
            if (CLRResource.OSGAdapt.Wrapper == null || CLRResource.OSGAdapt.Wrapper.Render(DrawingArea.Handle) == false)
            {
                Log.Error("Init Render Fail!");
            }
        }

        private void RichTextBox_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                // has a cale

                System.Windows.Controls.RichTextBox rtx = (System.Windows.Controls.RichTextBox)sender;
                System.Windows.Documents.TextRange textRange = new System.Windows.Documents.TextRange(rtx.Document.ContentStart, rtx.Document.ContentEnd);

                CLRResource.OSGAdapt.Wrapper.SetModelPath(textRange.Text);
            }
        }

        private void RichTextBox_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                // has a cale

                System.Windows.Controls.RichTextBox rtx = (System.Windows.Controls.RichTextBox)sender;
                System.Windows.Documents.TextRange textRange = new System.Windows.Documents.TextRange(rtx.Document.ContentStart, rtx.Document.ContentEnd);
                //Wrapper.Cale(rtx.Text);
                CLRResource.OSGAdapt.Wrapper.SetModelPath(textRange.Text);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            ITheme cur = System.Windows.Application.Current.Resources.GetTheme();

            
        }
    }
}
