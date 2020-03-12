using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace WPFGUI.ViewModel
{
    class ParamGridsViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public ParamGridsViewModel()
        {
            GenTestData();
        }

        private bool? _isAllItemsSelected;
        public bool? IsAllItemsSelected
        {
            get { return _isAllItemsSelected; }
            set
            {
                if (_isAllItemsSelected == value) return;

                _isAllItemsSelected = value;

                if (_isAllItemsSelected.HasValue)
                    SelectAll(_isAllItemsSelected.Value);

                OnPropertyChanged();
            }
        }

        private void SelectAll(bool value)
        {
            foreach (var p in _ParamItems)
            {
                p.IsSelected = value;
            }
        }

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }

        public ObservableCollection<ParamViewModel> _ParamItems = new ObservableCollection<ParamViewModel>();
        public ObservableCollection<ParamViewModel> ParamItems
        {
            get => _ParamItems;
            set
            {
                _ParamItems = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ParamItems)));
            }
        }

        void GenTestData()
        {
            _ParamItems.Add(new ParamViewModel() { Name = "W", Formula = "800" });
            _ParamItems.Add(new ParamViewModel() { Name = "D", Formula = "600" });
            _ParamItems.Add(new ParamViewModel() { Name = "H", Formula = "18" });
        }

        public void RemoveSelected()
        {
            var newItems = new ObservableCollection<ParamViewModel>();
            foreach (var item in _ParamItems)
            {
                if (item.IsSelected)
                {
                    continue;
                }
                newItems.Add(item);
            }
            ParamItems = newItems;
        }
        public string GetParamsJson()
        {
            return "";
        }
    }
}
