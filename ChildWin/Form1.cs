using System;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace ChildWin
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        [DllImport("USER32.DLL", CharSet = CharSet.Unicode)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("USER32.DLL")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, IntPtr lParam);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        [DllImport("user32")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool EnumChildWindows(IntPtr window, EnumWindowProc callback, IntPtr i);

        public const Int32 WM_COMMAND = 0x00000111;
        public const Int32 WM_LBUTTONDOWN = 0x00000201;
        public const Int32 WM_LBUTTONUP = 0x00000202;    

        [Flags]
        enum MIIM
        {
            BITMAP = 0x00000080,
            CHECKMARKS = 0x00000008,
            DATA = 0x00000020,
            FTYPE = 0x00000100,
            ID = 0x00000002,
            STATE = 0x00000001,
            STRING = 0x00000040,
            SUBMENU = 0x00000004,
            TYPE = 0x00000010
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        class MENUITEMINFO
        {
            public Int32 cbSize = Marshal.SizeOf(typeof(MENUITEMINFO));
            public MIIM fMask;
            public UInt32 fType;
            public UInt32 fState;
            public UInt32 wID;
            public IntPtr hSubMenu;
            public IntPtr hbmpChecked;
            public IntPtr hbmpUnchecked;
            public IntPtr dwItemData;
            public string dwTypeData = null;
            public UInt32 cch; // length of dwTypeData
            public IntPtr hbmpItem;

            public MENUITEMINFO() { }
            public MENUITEMINFO(MIIM pfMask)
            {
                fMask = pfMask;
            }
        }

        public delegate bool EnumWindowProc(IntPtr hWnd, IntPtr lParam);

        List<IntPtr> allChildWindows;
        List<IntPtr> listButton =  new List<IntPtr>();

        string[] buttonName =
        {
            "MC", "bcks", "7", "4", "1", "0",  "MR",
            "CE", "8", "5", "2", "MS", "C", "9", "6",
            "3", ",", "M+", "+-",  "/", "*", "-", "+",
            "M-", "sqrt", "%", "1/x", "="
        };

        private (int Index, string Button)[] buttonTuple = new (int Index, string Button)[]
        {
            (0,"MC"), (1,"bcks"), (2,"7"), (3,"4"),
            (4,"1"), (5,"0"), (6,"MR"), (7,"CE"),
            (8,"8"), (9,"5"), (10,"2"), (11,"MS"),
            (12,"C"), (13,"9"), (14,"6"), (15,"3"),
            (16,","), (17,"M+"), (18,"+-"), (19,"/"),
            (20,"*"), (21,"-"), (22,"+"), (23,"M-"),
            (24,"Корень"), (25,"%"), (26,"Дробь"), (27,"=")
        };
        /*
        public static List<IntPtr> GetChildWindows(IntPtr parent)
        {
            List<IntPtr> result = new List<IntPtr>();
            GCHandle listHandle = GCHandle.Alloc(result);
            try
            {
                EnumWindowProc childProc = new EnumWindowProc(EnumWindow);
                EnumChildWindows(parent, childProc, GCHandle.ToIntPtr(listHandle));
            }
            finally
            {
                if (listHandle.IsAllocated)
                    listHandle.Free();
            }
            return result;
        }

        private static bool EnumWindow(IntPtr handle, IntPtr pointer)
        {
            GCHandle gch = GCHandle.FromIntPtr(pointer);
            List<IntPtr> list = gch.Target as List<IntPtr>;
            if (list == null)
            {
                throw new InvalidCastException("GCHandle Target could not be cast as List<IntPtr>");
            }
            list.Add(handle);
            return true;
        }*/

        private void button1_Click(object sender, EventArgs e)
        {
            IntPtr calculatorHandle = FindWindow("CalcFrame", "Калькулятор");
            if (calculatorHandle == IntPtr.Zero)
            {
                MessageBox.Show("Calculator is not running.");
                return;
            }
            SetForegroundWindow(calculatorHandle);
            allChildWindows = new WindowHandleInfo(calculatorHandle).GetAllChildHandles();
            this.comboBox1.Items.Clear();
            StringBuilder szItemName = new StringBuilder();
            StringBuilder ClassName = new StringBuilder(256);
            foreach (var window in allChildWindows)
            {
                int countName = GetClassName(window, ClassName, ClassName.Capacity);
                if (countName != 0 && ClassName.ToString() == "Button")
                {
                    
                    listButton.Add(window);
                }
            }
            foreach (var button in this.buttonTuple.OrderBy(i => i.Button))
            {
                this.comboBox1.Items.Add(button.Button);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (this.comboBox1.Text.ToString().Length != 0)
            {
                SendMessage(listButton[this.buttonTuple.OrderBy(i => i.Button).ToArray()[this.comboBox1.SelectedIndex].Index], WM_LBUTTONDOWN, 0, IntPtr.Zero);
                SendMessage(listButton[this.buttonTuple.OrderBy(i => i.Button).ToArray()[this.comboBox1.SelectedIndex].Index], WM_LBUTTONUP, 0, IntPtr.Zero);
            }
        }
    }
}//public const UInt32 MF_BYCOMMAND = 0x00000000;
//public const UInt32 MF_BYPOSITION = 0x00000400;
//public const Int32 WM_GETTEXTLENGTH = 0x0000000D;
//public const Int32 WM_GETTEXT = 0x0000000E;

//[DllImport("user32.dll")]
//static extern IntPtr GetMenu(IntPtr hWnd);

//[DllImport("user32.dll", CharSet = CharSet.Auto)]
//static extern bool GetMenuItemInfo(IntPtr hMenu, UInt32 uItem, bool fByPosition, [In, Out] MENUITEMINFO lpmii);


//[DllImport("user32.dll", SetLastError = true)]
//static extern uint GetDlgItemText(IntPtr hDlg, int nIDDlgItem, [Out] StringBuilder lpString, int nMaxCount);

//[DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
//static extern int GetWindowTextLength(IntPtr hWnd);

//[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
//static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

//[DllImport("user32.dll", CharSet = CharSet.Auto)]
//static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, [Out] StringBuilder lParam);