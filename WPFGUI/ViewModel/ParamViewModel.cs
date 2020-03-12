using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace WPFGUI.ViewModel
{
    class ParamViewModel : INotifyPropertyChanged
    {
        public ParamViewModel()
        {

        }
        private bool _IsSelected = false;
        public bool IsSelected
        {
            get => _IsSelected;
            set
            {
                if (_IsSelected == value)
                    return;
                _IsSelected = value;
                OnPropertyChanged();
            }
        }
        private string _Name = string.Empty;
        public string Name
        {
            get => _Name;
            set
            {
                if (_Name == value)
                    return;
                _Name = value;
                OnPropertyChanged();
            }
        }
        private string _Formula = string.Empty;
        public string Formula
        {
            get => _Formula;
            set
            {
                if (_Formula == value)
                    return;
                _Formula = value;
                OnPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
