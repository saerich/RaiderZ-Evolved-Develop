﻿using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace UnitTestTool
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            if (true == OverlappedExecuteChecker.Checker.Check())
            {
                return;
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}