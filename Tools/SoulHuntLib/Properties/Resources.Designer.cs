﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace SoulHunt.Properties {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "16.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    public class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("SoulHunt.Properties.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;content_table&quot;&gt;
        ///	&lt;tr class=&quot;grayed&quot;&gt;&lt;td&gt;[[NPC_NAME]] (&lt;a href=&quot;javascript:window.external.Navigate(&apos;NPC&apos;, &apos;[[NPC_ID]]&apos;)&quot;&gt;[[NPC_ID]]&lt;/a&gt;)&lt;/td&gt;&lt;/tr&gt;
        ///&lt;/table&gt;.
        /// </summary>
        public static string dialog_npc_template {
            get {
                return ResourceManager.GetString("dialog_npc_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;dialog_table&quot;&gt;
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;dialog_text&quot;&gt;
        ///	&lt;p&gt;[[DIALOG_TEXT]]&lt;/p&gt;
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///&lt;/table&gt;
        ///&lt;br/&gt;
        ///.
        /// </summary>
        public static string dialog_preview_element_template {
            get {
                return ResourceManager.GetString("dialog_preview_element_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;dialog_table&quot;&gt;
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;dialog_npc_name&quot;&gt;
        ///&lt;P&gt;NPC 이름&lt;/P&gt;
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///[[DIALOG_TEXT_ELEMENT]]
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;dialog_text&quot;&gt;
        ///----------------------------------------------------------------------
        ///&lt;/div&gt;
        ///&lt;div class=&quot;dialog_select&quot;&gt;
        ///▶ 선택1&lt;br/&gt;
        ///▶ 선택2
        ///&lt;/div&gt;
        ///&lt;br/&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///&lt;/table&gt;
        ///
        ///&lt;br/&gt;
        ///
        ///.
        /// </summary>
        public static string dialog_preview_template {
            get {
                return ResourceManager.GetString("dialog_preview_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;h1&gt;[[DIALOG_TEXT]] (&lt;a href=&quot;javascript:window.external.CopyString(&apos;[[ID]]&apos;)&quot;&gt;[[ID]]&lt;/a&gt;)&lt;/h1&gt;
        ///
        ///.
        /// </summary>
        public static string dialog_template {
            get {
                return ResourceManager.GetString("dialog_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;div class=&quot;error&quot;&gt;
        ///	&lt;p&gt;이 컨텐츠는 에러를 포함하고 있습니다. 원본 파일을 수정 하시면 에러가 자동복구됩니다.&lt;/p&gt;
        ///	&lt;p&gt;[[Message]]&lt;/p&gt;
        ///&lt;/div&gt;.
        /// </summary>
        public static string error_template {
            get {
                return ResourceManager.GetString("error_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;tr&gt;
        ///	&lt;td&gt;[[LOOT_DETAIL]]&lt;/td&gt;
        ///	&lt;td&gt;[[LOOT_RATE]]&lt;/td&gt;
        ///	&lt;td&gt;[[LOOT_MIN]]&lt;/td&gt;
        ///	&lt;td&gt;[[LOOT_MAX]]&lt;/td&gt;
        ///&lt;/tr&gt;.
        /// </summary>
        public static string loot_element_template {
            get {
                return ResourceManager.GetString("loot_element_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;h2&gt;전리품 목록&lt;/h2&gt;
        ///
        ///&lt;ul&gt;
        ///	&lt;li&gt;아이디: &lt;a href=&quot;javascript:window.external.Navigate(&apos;LOOTING&apos;, &apos;[[LOOT_ID]]&apos;)&quot;&gt;[[LOOT_ID]]&lt;/a&gt;&lt;/li&gt;
        ///	&lt;li&gt;최대갯수: [[LOOT_MAX]]&lt;/li&gt;
        ///&lt;/ul&gt;
        ///
        ///&lt;table class=&quot;content_table&quot;&gt;
        ///&lt;tr&gt;
        ///	&lt;th&gt;내용&lt;/th&gt;
        ///	&lt;th&gt;확률&lt;/th&gt;
        ///	&lt;th&gt;최소&lt;/th&gt;
        ///	&lt;th&gt;최대&lt;/th&gt;
        ///&lt;/tr&gt;
        ///	[[LOOT_ELEMENTS]]
        ///&lt;/table&gt;.
        /// </summary>
        public static string loot_template {
            get {
                return ResourceManager.GetString("loot_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;content_table&quot;&gt;
        ///	&lt;tr class=&quot;grayed&quot;&gt;&lt;td&gt;조건: [[Condition]]&lt;/td&gt;&lt;/tr&gt;
        ///	&lt;tr&gt;&lt;td&gt;[[Interaction]]&lt;/td&gt;&lt;/tr&gt;
        ///	&lt;tr&gt;&lt;td&gt;&lt;q&gt;&lt;span class=&quot;gray&quot;&gt;[[Details]]&lt;/span&gt;&lt;/q&gt;&lt;/td&gt;&lt;/tr&gt;
        ///&lt;/table&gt;.
        /// </summary>
        public static string npc_interaction_template {
            get {
                return ResourceManager.GetString("npc_interaction_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;h1&gt;[[Name]][[Clan]] (&lt;a href=&quot;javascript:window.external.CopyString(&apos;[[ID]]&apos;)&quot;&gt;[[ID]]&lt;/a&gt;)&lt;/h1&gt;
        ///&lt;ul&gt;
        ///	&lt;li&gt;타입: [[Type]]&lt;/li&gt;
        ///	&lt;li&gt;레벨: [[Level]]&lt;/li&gt;
        ///	&lt;li&gt;등급: [[Grade]]&lt;/li&gt;
        ///&lt;/ul&gt;
        ///.
        /// </summary>
        public static string npc_template {
            get {
                return ResourceManager.GetString("npc_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;content_table&quot;&gt;
        ///	&lt;tr class=&quot;grayed&quot;&gt;&lt;td&gt;[[NPC_NAME]] (&lt;a href=&quot;javascript:window.external.Navigate(&apos;NPC&apos;, &apos;[[NPC_ID]]&apos;)&quot;&gt;[[NPC_ID]]&lt;/a&gt;) , [[QUEST_TYPE]]&lt;/td&gt;&lt;/tr&gt;
        ///&lt;/table&gt;.
        /// </summary>
        public static string quest_npc_template {
            get {
                return ResourceManager.GetString("quest_npc_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;content_table&quot;&gt;
        ///	&lt;tr class=&quot;grayed&quot;&gt;&lt;td&gt;[[QUEST_OBJ_ID]] , [[QUEST_OBJ_DESC]]&lt;/td&gt;&lt;/tr&gt;
        ///	&lt;tr&gt;
        ///		&lt;td&gt;
        ///			&lt;UL&gt;
        ///&lt;!--				&lt;LI&gt;type: [[QUEST_OBJ_TYPE]]&lt;/LI&gt; --&gt;
        ///				&lt;LI&gt;param1: [[QUEST_OBJ_PARAM1]]&lt;/LI&gt;
        ///				&lt;LI&gt;param2: [[QUEST_OBJ_PARAM2]]&lt;/LI&gt;
        ///&lt;!--				&lt;LI&gt;public_progress: [[QUEST_OBJ_PUBLIC_PROGRESS]]&lt;/LI&gt; --&gt;
        ///				&lt;LI&gt;field: [[QUEST_OBJ_FIELD]]&lt;/LI&gt;
        ///				&lt;LI&gt;marker: [[QUEST_OBJ_MARKER]]&lt;/LI&gt;
        ///			&lt;/UL&gt;
        ///		&lt;/TD&gt;
        ///	&lt;/TR&gt;
        ///&lt;/table&gt;.
        /// </summary>
        public static string quest_objective_template {
            get {
                return ResourceManager.GetString("quest_objective_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;quest_table&quot;&gt;
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;quest_title&quot;&gt;
        ///&lt;P&gt;[[QUEST_TITLE]]&lt;/P&gt;
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;quest_text&quot;&gt;
        ///[[QUEST_DESC]]
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;br/&gt;
        ///&lt;div class=&quot;quest_title&quot;&gt;
        ///&lt;P&gt;보상&lt;/P&gt;
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///&lt;tr&gt;
        ///&lt;td&gt;
        ///&lt;div class=&quot;quest_text&quot;&gt;
        ///다음의 보상을 확인하십시오.
        ///&lt;br/&gt;&lt;br/&gt;
        ///&lt;/div&gt;
        ///&lt;/td&gt;
        ///&lt;/tr&gt;
        ///
        ///&lt;/table&gt;
        ///
        ///&lt;br/&gt;
        ///
        ///.
        /// </summary>
        public static string quest_preview_template {
            get {
                return ResourceManager.GetString("quest_preview_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;table class=&quot;content_table&quot;&gt;
        ///	&lt;tr class=&quot;grayed&quot;&gt;
        ///	&lt;td&gt;
        ///		&lt;UL&gt;
        ///			&lt;LI&gt;dialog: [[QUEST_REWARD_DIALOG_NAME]](&lt;a href=&quot;javascript:window.external.Navigate(&apos;DIALOG&apos;, &apos;[[QUEST_REWARD_DIALOG_ID]]&apos;)&quot;&gt;[[QUEST_REWARD_DIALOG_ID]]&lt;/a&gt;)&lt;/LI&gt;
        ///			&lt;LI&gt;rewarder: [[QUEST_REWARD_REWARDER]]&lt;/LI&gt;
        ///		&lt;/UL&gt;
        ///	&lt;/td&gt;
        ///	&lt;/tr&gt;
        ///&lt;/table&gt;.
        /// </summary>
        public static string quest_reward_template {
            get {
                return ResourceManager.GetString("quest_reward_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;h1&gt;[[QUEST_TEXT]] (&lt;a href=&quot;javascript:window.external.CopyString(&apos;[[ID]]&apos;)&quot;&gt;[[ID]]&lt;/a&gt;)&lt;/h1&gt;
        ///
        ///&lt;h2&gt;NPC&lt;/h2&gt;
        ///
        ///[[QUEST_NPCS]]
        ///
        ///&lt;h2&gt;Objectives&lt;/h2&gt;
        ///
        ///[[QUEST_OBJECTIVES]]
        ///
        ///&lt;h2&gt;Rewards&lt;/h2&gt;
        ///
        ///[[QUEST_REWARDS]]
        ///
        ///&lt;h2&gt;Items&lt;/h2&gt;
        ///
        ///&lt;h2&gt;미리 보기&lt;/h2&gt;
        ///
        ///[[QUEST_PREVIEW]].
        /// </summary>
        public static string quest_template {
            get {
                return ResourceManager.GetString("quest_template", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;div&gt;
        ///	&lt;h1&gt;Velixian [[PRODUCT_VERSION]] 변경사항&lt;/h1&gt;
        ///	&lt;ul&gt;
        ///		&lt;li&gt;&lt;span class=&quot;warning&quot;&gt;대화 미리보기 기능을 추가하였습니다.(2011/03/11)&lt;/span&gt;&lt;/li&gt;
        ///		&lt;li&gt;&lt;span class=&quot;warning&quot;&gt;창에 도킹 기능을 추가하였습니다.(2011/03/06)&lt;/span&gt;&lt;/li&gt;
        ///		
        ///&lt;!--		
        ///		&lt;li&gt;조건 편집의 타입에 &apos;quest_objective_complete&apos;, &apos;quest_objective_incomplete&apos;을 사용할 수 있게 되었습니다.&lt;/li&gt;
        ///		&lt;li&gt;퀘스트 편집의 타입에 &apos;SCOUT&apos;를 사용할 수 있게 되었습니다.&lt;/li&gt;
        ///		&lt;li&gt;&lt;span class=&quot;warning&quot;&gt;이제 조건을 편집하고 저장할 수 있습니다.&lt;/span&gt;&lt;/li&gt;
        ///		&lt;li&gt;&lt;span class=&quot;warning&quot;&gt;문자열 검색이 보다 개선되었습니다. (검색 옵션이 늘어났습니다.)&lt;/span&gt;
        ///			&lt;ul&gt;
        ///		 [rest of string was truncated]&quot;;.
        /// </summary>
        public static string WhatsNew {
            get {
                return ResourceManager.GetString("WhatsNew", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.1//EN&quot; &quot;http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd&quot;&gt;
        ///&lt;html xml:lang=&quot;ko&quot; xmlns=&quot;http://www.w3.org/1999/xhtml&quot;&gt;
        ///	&lt;head&gt;
        ///		&lt;title&gt;.&lt;/title&gt;
        ///		&lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot; /&gt;
        ///		&lt;style type=&quot;text/css&quot;&gt;
        ///			body { background-color: #fff; color: #303030; padding: 5px; margin: 0; font-family: &quot;돋움&quot;; font-size: 12px; line-height: 1.5em; }
        ///			h1 { font-size: 16px; font-weight: bold; color: #0072bc; width: 100%; border-bottom: 2px [rest of string was truncated]&quot;;.
        /// </summary>
        public static string xhtml_template {
            get {
                return ResourceManager.GetString("xhtml_template", resourceCulture);
            }
        }
    }
}
