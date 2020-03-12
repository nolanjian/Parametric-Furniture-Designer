using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPFGUI.ViewModel;

namespace WPFGUI
{
    /// <summary>
    /// Interaction logic for ParamGrids.xaml
    /// </summary>
    public partial class ParamGrids : UserControl
    {
        private ParamGridsViewModel paramGridsViewModel { get; set; } = null;
        public ParamGrids()
        {
            InitializeComponent();
            paramGridsViewModel = (ParamGridsViewModel)DataContext;
        }

        private void Button_Click_Add(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_Del(object sender, RoutedEventArgs e)
        {
            paramGridsViewModel.RemoveSelected();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }
        private void Button_Click_Recover(object sender, RoutedEventArgs e)
        {
        }
        private void Button_Click_Save(object sender, RoutedEventArgs e)
        {
            CLRResource.OSGAdapt.Wrapper.CleanParam();
            CLRResource.OSGAdapt.Wrapper.SetParams(-1, paramGridsViewModel.GetParamsJson());
        }
    }
}
