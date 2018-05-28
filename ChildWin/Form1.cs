using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections;

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

        [DllImport("user32.dll")]
        static extern IntPtr GetMenu(IntPtr hWnd);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern bool GetMenuItemInfo(IntPtr hMenu, UInt32 uItem, bool fByPosition, [In, Out] MENUITEMINFO lpmii);
        

        [DllImport("user32.dll", SetLastError = true)]
        static extern uint GetDlgItemText(IntPtr hDlg, int nIDDlgItem, [Out] StringBuilder lpString, int nMaxCount);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern int GetWindowTextLength(IntPtr hWnd);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, IntPtr lParam);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, [Out] StringBuilder lParam);

        public const Int32 WM_COMMAND = 0x00000111;
        public const Int32 WM_LBUTTONDOWN = 0x00000201;
        public const Int32 WM_LBUTTONUP = 0x00000202;
        public const UInt32 MF_BYCOMMAND = 0x00000000;
        public const UInt32 MF_BYPOSITION = 0x00000400;
        public const Int32 WM_GETTEXTLENGTH = 0x0000000D;
        public const Int32 WM_GETTEXT = 0x0000000E;
        List<IntPtr> allChildWindows;
        /*
        Public Const WM_CHAR = &H102;
Public Const WM_SETTEXT = &HC;
Public Const WM_KEYDOWN = &H100;
Public Const WM_KEYUP = &H101;
Public Const WM_LBUTTONDOWN = &H201;
Public Const WM_LBUTTONUP = &H202;
Public Const WM_CLOSE = &H10;
Public Const WM_COMMAND = &H111;
Public Const WM_CLEAR = &H303;
Public Const WM_DESTROY = &H2;
Public Const WM_GETTEXT = &HD;
Public Const WM_GETTEXTLENGTH = &HE;
Public Const WM_LBUTTONDBLCLK = &H203;*/
        /*
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool EnumChildWindows(IntPtr hwndParent, EnumWindowsProc lpEnumFunc, IntPtr lParam);*/
        [DllImport("user32")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool EnumChildWindows(IntPtr window, EnumWindowProc callback, IntPtr i);

        //public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
        public delegate bool EnumWindowProc(IntPtr hWnd, IntPtr lParam);
        //private delegate bool EnumedWindow(IntPtr handleWindow, ArrayList handles);

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

        private void button1_Click(object sender, EventArgs e)
        {
            //IntPtr calculatorHandle = FindWindow(null, "Калькулятор ‎- Калькулятор");
            IntPtr calculatorHandle = FindWindow("CalcFrame", "Калькулятор");
            if (calculatorHandle == IntPtr.Zero)
            {
                MessageBox.Show("Calculator is not running.");
                return;
            }
            SetForegroundWindow(calculatorHandle);
            /* foreach (var window in GetChildWindows(calculatorHandle))
             {
                 this.comboBox1.Items.Add(window.ToString());
             }*/
            allChildWindows = new WindowHandleInfo(calculatorHandle).GetAllChildHandles();
            this.comboBox1.Items.Clear();
            StringBuilder szItemName = new StringBuilder();
            foreach (var window in allChildWindows)
            {
                //this.comboBox1.Items.Add(window.ToString());
                this.comboBox1.Items.Add(GetText(window));
                //GetDlgItemText(window, 0, szItemName, 80);
                //this.comboBox1.Items.Add(szItemName.ToString());
                //SendMessage(window, WM_COMMAND, 0, IntPtr.Zero);
            }
            
            /*
            IntPtr hMenu = GetMenu(calculatorHandle);
            this.comboBox1.Items.Add(hMenu.ToString());
            MENUITEMINFO mif = new MENUITEMINFO(MIIM.STRING | MIIM.ID);
            bool res = GetMenuItemInfo(hMenu, 0, true, mif);
            if (res)
            {
                ++mif.cch;
                mif.dwTypeData = new String(' ', (int)mif.cch);
                res = GetMenuItemInfo(hMenu, 0, true, mif);
                this.comboBox1.Items.Add(mif.dwItemData);
            }*/
        }

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
            //  You can modify this to check to see if you want to cancel the operation, then return a null here
            return true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SendMessage(allChildWindows[this.comboBox1.SelectedIndex], WM_LBUTTONDOWN, 0, IntPtr.Zero);
            SendMessage(allChildWindows[this.comboBox1.SelectedIndex], WM_LBUTTONUP, 0, IntPtr.Zero);
        }

        public static string GetText(IntPtr hWnd)
        {
            // Allocate correct string length first
            int length = GetWindowTextLength(hWnd);
            StringBuilder sb = new StringBuilder(length + 1);
            GetWindowText(hWnd, sb, sb.Capacity);
            return sb.ToString();
        }

        public static string GetWindowTextRaw(IntPtr hwnd)
        {
            int length = (int)SendMessage(hwnd, WM_GETTEXTLENGTH, 0, IntPtr.Zero);
            StringBuilder sb = new StringBuilder(length + 1);
            SendMessage(hwnd, WM_GETTEXT, (IntPtr)sb.Capacity, sb);
            return sb.ToString();
        }
    }
}
