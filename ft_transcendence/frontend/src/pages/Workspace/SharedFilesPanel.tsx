import React, { useState, useRef, useEffect } from 'react';
import { fetchBlobUrl, downloadFile } from '../../api';
import {
  FileText, Terminal, Code2, Settings2,
  Trash2, Download, Copy, Check, Search, Loader2, AlertCircle, UploadCloud, X, ZoomIn,
  Braces, Globe, Coffee, Database, FileImage, Archive, Table, Wrench, FileJson, Box,
} from 'lucide-react';

const ALLOWED_EXTENSIONS = new Set([
  'jpg','jpeg','png','gif','webp','svg','ico','bmp',
  'pdf','md','mdx','rst','tex',
  'py','js','ts','tsx','jsx','html','htm','css','scss','sass','less','vue','svelte',
  'c','cpp','cc','cxx','h','hpp','java','kt','go','rs','rb','php','swift',
  'sh','bash','zsh','fish','ps1','bat',
  'json','yaml','yml','toml','ini','conf','env','xml','csv','tsv','sql','txt','log',
  'zip','tar','gz','bz2','xz','7z','tgz',
  'ipynb','dockerfile','makefile',
]);

const IMAGE_EXTENSIONS = new Set(['jpg','jpeg','png','gif','webp','svg','bmp','ico']);
const PDF_EXTENSIONS   = new Set(['pdf']);

export interface FileItem {
  id: number;
  name: string;
  size: string;
  type: string;
  uploadedAt?: string;
  url?: string;
  status?: 'loading' | 'success';
  progress?: number;
}

interface SharedFilesProps {
  files: FileItem[];
  onUpload: (file: File) => Promise<void>;
  onDelete: (id: number) => void;
  uploadTriggerRef?: React.MutableRefObject<(() => void) | null>;
}

export function SharedFiles({ files, onUpload, onDelete, uploadTriggerRef }: SharedFilesProps) {
  const fileInputRef = useRef<HTMLInputElement>(null);

  useEffect(() => {
    if (uploadTriggerRef) uploadTriggerRef.current = () => fileInputRef.current?.click();
  }, [uploadTriggerRef]);
  const [isDragging, setIsDragging] = useState(false);
  const [searchQuery, setSearchQuery] = useState('');
  const [errorMsg, setErrorMsg] = useState<string | null>(null);
  const [deleteConfirmId, setDeleteConfirmId] = useState<number | null>(null);
  const [copiedId, setCopiedId] = useState<number | null>(null);
  const [previewFile, setPreviewFile] = useState<FileItem | null>(null);
  const [previewBlobUrl, setPreviewBlobUrl] = useState<string | null>(null);
  const [previewLoading, setPreviewLoading] = useState(false);
  const [previewError, setPreviewError] = useState(false);

  const detectFileType = (fileName: string): string => {
    const lowerName = fileName.toLowerCase();
    const ext = lowerName.split('.').pop() || '';

    if (['cpp', 'cc', 'cxx', 'hpp', 'hxx', 'inl'].includes(ext))             return 'cpp';
    if (['c', 'h'].includes(ext))                                              return 'c';
    if (['py', 'pyw', 'pyi', 'ipynb'].includes(ext))                          return 'python';
    if (lowerName.endsWith('.d.ts') || ['ts', 'tsx'].includes(ext))           return 'typescript';
    if (['js', 'jsx', 'mjs', 'cjs'].includes(ext))                            return 'javascript';
    if (['html', 'htm', 'css', 'scss', 'sass', 'less', 'svelte', 'vue'].includes(ext)) return 'web';
    if (['java', 'class', 'jar', 'kt', 'kts', 'scala', 'groovy'].includes(ext)) return 'java';
    if (ext === 'go')                                                          return 'go';
    if (ext === 'rs')                                                          return 'rust';
    if (['rb', 'erb'].includes(ext) || lowerName === 'gemfile')               return 'ruby';
    if (ext === 'php')                                                         return 'php';
    if (['swift', 'm', 'mm'].includes(ext))                                   return 'swift';
    if (['sh', 'bash', 'zsh', 'fish', 'ps1', 'bat', 'cmd'].includes(ext))    return 'shell';
    if (ext === 'pdf')                                                         return 'pdf';
    if (lowerName === 'dockerfile' || ext === 'dockerignore' ||
        lowerName.startsWith('docker-compose') ||
        lowerName === 'compose.yml' || lowerName === 'compose.yaml')          return 'docker';
    if (['mk', 'cmake'].includes(ext) ||
        ['makefile', 'doxyfile', 'cmakelists.txt', 'gradlew', 'justfile'].includes(lowerName))
      return 'build';
    if (['json', 'jsonc', 'json5'].includes(ext))                             return 'json';
    if (['yaml', 'yml', 'toml', 'ini', 'conf', 'env', 'properties', 'xml'].includes(ext) ||
        lowerName.startsWith('.env'))                                          return 'config';
    if (['sql', 'db', 'sqlite', 'sqlite3'].includes(ext))                     return 'database';
    if (['png', 'jpg', 'jpeg', 'gif', 'webp', 'ico', 'bmp', 'tiff', 'svg'].includes(ext)) return 'image';
    if (['zip', 'tar', 'gz', 'bz2', 'xz', 'rar', '7z', 'tgz'].includes(ext)) return 'archive';
    if (['csv', 'tsv', 'parquet', 'ndjson'].includes(ext))                    return 'data';
    if (['md', 'mdx', 'rst', 'tex', 'adoc'].includes(ext))                   return 'markdown';
    return 'text';
  };

  const FILE_ICON: Record<string, React.ReactNode> = {
    cpp:        <Code2     size={13} className="text-blue-600"    />,
    c:          <Code2     size={13} className="text-blue-400"    />,
    python:     <Code2     size={13} className="text-yellow-500"  />,
    typescript: <Braces    size={13} className="text-blue-600"    />,
    javascript: <Braces    size={13} className="text-yellow-500"  />,
    web:        <Globe     size={13} className="text-orange-500"  />,
    java:       <Coffee    size={13} className="text-red-700"     />,
    go:         <Code2     size={13} className="text-cyan-500"    />,
    rust:       <Code2     size={13} className="text-orange-600"  />,
    ruby:       <Code2     size={13} className="text-red-500"     />,
    php:        <Code2     size={13} className="text-indigo-500"  />,
    swift:      <Code2     size={13} className="text-orange-400"  />,
    shell:      <Terminal  size={13} className="text-amber-500"   />,
    pdf:        <FileText  size={13} className="text-red-500"     />,
    docker:     <Box       size={13} className="text-sky-500"     />,
    build:      <Wrench    size={13} className="text-green-600"   />,
    json:       <FileJson  size={13} className="text-violet-500"  />,
    config:     <Settings2 size={13} className="text-purple-500"  />,
    database:   <Database  size={13} className="text-emerald-600" />,
    image:      <FileImage size={13} className="text-pink-500"    />,
    archive:    <Archive   size={13} className="text-orange-500"  />,
    data:       <Table     size={13} className="text-teal-500"    />,
    markdown:   <FileText  size={13} className="text-sky-500"     />,
    text:       <FileText  size={13} className="text-gray-400"    />,
  };

  const FILE_LABEL: Record<string, string> = {
    cpp: 'C++', c: 'C', python: 'PY', typescript: 'TS', javascript: 'JS',
    web: 'WEB', java: 'JAVA', go: 'GO', rust: 'RS', ruby: 'RB',
    php: 'PHP', swift: 'SWIFT', shell: 'SH', pdf: 'PDF', docker: 'DOCKER', build: 'MAKE',
    json: 'JSON', config: 'CFG', database: 'SQL', image: 'IMG',
    archive: 'ZIP', data: 'CSV', markdown: 'MD', text: 'TXT',
  };

  const FILE_BADGE: Record<string, string> = {
    cpp:        'bg-blue-100 text-blue-700',
    c:          'bg-blue-50 text-blue-500',
    python:     'bg-yellow-100 text-yellow-700',
    typescript: 'bg-blue-100 text-blue-700',
    javascript: 'bg-yellow-100 text-yellow-600',
    web:        'bg-orange-100 text-orange-600',
    java:       'bg-red-100 text-red-700',
    go:         'bg-cyan-100 text-cyan-700',
    rust:       'bg-orange-100 text-orange-700',
    ruby:       'bg-red-100 text-red-600',
    php:        'bg-indigo-100 text-indigo-600',
    swift:      'bg-orange-50 text-orange-500',
    shell:      'bg-amber-100 text-amber-600',
    pdf:        'bg-red-100 text-red-600',
    docker:     'bg-sky-100 text-sky-600',
    build:      'bg-green-100 text-green-700',
    json:       'bg-violet-100 text-violet-600',
    config:     'bg-purple-100 text-purple-600',
    database:   'bg-emerald-100 text-emerald-700',
    image:      'bg-pink-100 text-pink-600',
    archive:    'bg-orange-100 text-orange-600',
    data:       'bg-teal-100 text-teal-600',
    markdown:   'bg-sky-100 text-sky-600',
    text:       'bg-gray-200/50 text-gray-500',
  };

  const getFileIcon  = (type: string) => FILE_ICON[type]  ?? FILE_ICON.text;
  const getFileLabel = (type: string) => FILE_LABEL[type] ?? type.toUpperCase();
  const getFileBadge = (type: string) => FILE_BADGE[type] ?? 'bg-gray-200/50 text-gray-500';

  const getExt  = (name: string) => name.toLowerCase().split('.').pop() ?? '';
  const isImage = (name: string) => IMAGE_EXTENSIONS.has(getExt(name));
  const isPdf   = (name: string) => PDF_EXTENSIONS.has(getExt(name));

  const MAX_SIZE = 5 * 1024 * 1024;

  const validateAndUpload = (fileList: FileList) => {
    setErrorMsg(null);
    const errors: string[] = [];

    Array.from(fileList).forEach((file) => {
      const ext = getExt(file.name);
      const baseName = file.name.toLowerCase();
      if (!ALLOWED_EXTENSIONS.has(ext) && !ALLOWED_EXTENSIONS.has(baseName)) {
        errors.push(`"${file.name}" — unsupported type`);
        return;
      }
      if (file.size > MAX_SIZE) {
        const mb = (file.size / (1024 * 1024)).toFixed(1);
        errors.push(`"${file.name}" — file is ${mb} MB, limit is 5 MB`);
        return;
      }
      if (files.some(f => f.name.toLowerCase() === file.name.toLowerCase())) {
        errors.push(`"${file.name}" — already exists`);
        return;
      }
      onUpload(file).catch((err: unknown) => {
        const msg = err instanceof Error ? err.message : 'Upload failed';
        setErrorMsg(`"${file.name}" — ${msg}`);
      });
    });

    if (errors.length > 0) {
      setErrorMsg(errors[0]);
    }
  };

  const handleDragOver = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(true);
  };

  const handleDragLeave = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
  };

  const handleDrop = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
    if (e.dataTransfer.files && e.dataTransfer.files.length > 0) {
      validateAndUpload(e.dataTransfer.files);
    }
  };

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (e.target.files && e.target.files.length > 0) {
      validateAndUpload(e.target.files);
      e.target.value = '';
    }
  };

  const handleCopyName = (e: React.MouseEvent, file: FileItem) => {
    e.stopPropagation();
    navigator.clipboard.writeText(file.name).then(() => {
      setCopiedId(file.id);
      setTimeout(() => setCopiedId(null), 2000);
    });
  };

  const handleDownload = (e: React.MouseEvent, file: FileItem) => {
    e.stopPropagation();
    if (!file.url) return;
    downloadFile(file.url, file.name).catch(() => {});
  };

  const openPreview = (e: React.MouseEvent, file: FileItem) => {
    e.stopPropagation();
    setPreviewFile(file);
    setPreviewBlobUrl(null);
    setPreviewError(false);
    if (!file.url) return;
    setPreviewLoading(true);
    fetchBlobUrl(file.url)
      .then(url => { setPreviewBlobUrl(url); setPreviewLoading(false); })
      .catch(() => { setPreviewLoading(false); setPreviewError(true); });
  };

  const closePreview = () => {
    setPreviewBlobUrl(prev => { if (prev) URL.revokeObjectURL(prev); return null; });
    setPreviewFile(null);
    setPreviewLoading(false);
    setPreviewError(false);
  };

  const filteredFiles = files.filter((file) =>
    file.name.toLowerCase().includes(searchQuery.toLowerCase())
  );

  return (
    <section 
      onDragOver={handleDragOver}
      onDragLeave={handleDragLeave}
      onDrop={handleDrop}
      className={`w-full h-full flex flex-col transition-all duration-200 relative select-none ${
        isDragging ? 'rounded-xl border-dashed border-2 border-indigo-500 bg-indigo-50/20 scale-[1.005]' : ''
      }`}
    >
      <input
        type="file"
        ref={fileInputRef}
        onChange={handleFileChange}
        className="hidden"
        multiple
        accept=".jpg,.jpeg,.png,.gif,.webp,.svg,.ico,.bmp,.pdf,.md,.mdx,.rst,.tex,.py,.js,.ts,.tsx,.jsx,.html,.htm,.css,.scss,.sass,.less,.vue,.svelte,.c,.cpp,.cc,.cxx,.h,.hpp,.java,.kt,.go,.rs,.rb,.php,.swift,.sh,.bash,.zsh,.fish,.ps1,.bat,.json,.yaml,.yml,.toml,.ini,.conf,.env,.xml,.csv,.tsv,.sql,.txt,.log,.zip,.tar,.gz,.bz2,.xz,.7z,.tgz,.ipynb,Dockerfile,Makefile"
      />

      {isDragging && (
        <div className="absolute inset-0 bg-indigo-50/40 backdrop-blur-3xs rounded-xl flex flex-col items-center justify-center gap-2 z-30 pointer-events-none animate-fadeIn">
          <div className="w-12 h-12 rounded-full bg-indigo-100 flex items-center justify-center text-indigo-600 animate-bounce">
            <UploadCloud size={22} />
          </div>
          <span className="text-xs font-bold text-indigo-600 tracking-wide">Drop files here...</span>
        </div>
      )}


      <div className="relative mb-3 flex-shrink-0 z-10">
        <Search className="absolute left-2.5 top-2.5 text-gray-400" size={13} />
        <input 
          type="text" 
          placeholder="Search files..."
          value={searchQuery}
          onChange={(e) => setSearchQuery(e.target.value)}
          className="w-full bg-gray-50/60 border border-gray-200/60 rounded-lg pl-8 pr-7 py-1.5 text-[11px] outline-none focus:border-indigo-500 focus:bg-white transition-all"
        />
        {searchQuery && (
          <button 
            onClick={() => setSearchQuery('')}
            className="absolute right-2.5 top-2.5 text-gray-400 hover:text-gray-600 transition-colors cursor-pointer"
          >
            <X size={13} />
          </button>
        )}
      </div>

      {errorMsg && (
        <div className="mb-3 p-2 bg-rose-50 border border-rose-100 rounded-lg flex items-center gap-1.5 text-[10px] text-rose-600 animate-fadeIn flex-shrink-0 z-10">
          <AlertCircle size={12} className="shrink-0" />
          <span className="font-medium truncate flex-1">{errorMsg}</span>
          <button onClick={() => setErrorMsg(null)} className="text-rose-400 hover:text-rose-600 font-bold ml-1 cursor-pointer">×</button>
        </div>
      )}
      
      <div 
        className="flex-1 overflow-y-auto space-y-1.5 pr-0.5 overflow-x-hidden z-10" 
        style={{ scrollbarWidth: 'none', msOverflowStyle: 'none' }}
      >
        {filteredFiles.length === 0 && (
          <div className="h-full flex flex-col items-center justify-center text-center p-4">
            <p className="text-[11px] text-gray-400">
              {searchQuery ? 'No matching files found' : 'Drag and drop your files here'}
            </p>
          </div>
        )}

        {filteredFiles.map(file => (
          <div
            key={file.id}
            className="flex items-center justify-between p-2.5 bg-gray-50/60 hover:bg-indigo-50/30 border border-gray-200/40 hover:border-indigo-100 rounded-lg transition-all group relative overflow-hidden"
          >
            {file.status === 'loading' && (
              <div className="absolute bottom-0 left-0 right-0 h-0.5 bg-gray-100">
                <div
                  className="h-full bg-indigo-400 transition-all duration-150"
                  style={{ width: `${file.progress ?? 0}%` }}
                />
              </div>
            )}
            <div className="flex items-center gap-2.5 min-w-0 flex-1">
              <div className="w-7 h-7 rounded bg-white border border-gray-200/60 flex items-center justify-center transition-colors flex-shrink-0 shadow-3xs group-hover:bg-white">
                {file.status === 'loading' ? (
                  <Loader2 size={13} className="text-indigo-500 animate-spin" />
                ) : (
                  getFileIcon(detectFileType(file.name))
                )}
              </div>
              <div className="truncate">
                <span className="block text-xs font-medium text-gray-700 truncate group-hover:text-indigo-950 transition-colors">
                  {file.name}
                </span>
                <span className="block text-[10px] text-gray-400 font-mono">{file.size}</span>
              </div>
            </div>

            <div className="flex items-center ml-2 flex-shrink-0 z-10">
              {file.status !== 'loading' && (
                <div className="hidden group-hover:flex items-center gap-0.5 bg-white border border-gray-100 rounded-md shadow-3xs p-0.5">
                  <button
                    onClick={(e) => handleCopyName(e, file)}
                    title="Copy filename"
                    className={`p-1 rounded hover:bg-gray-50 transition-colors cursor-pointer ${copiedId === file.id ? 'text-emerald-500' : 'text-gray-400 hover:text-gray-700'}`}
                  >
                    {copiedId === file.id ? <Check size={12} className="animate-scaleIn" /> : <Copy size={12} />}
                  </button>
                  {file.url && (isImage(file.name) || isPdf(file.name)) && (
                    <button
                      onClick={(e) => openPreview(e, file)}
                      title={isPdf(file.name) ? "Preview PDF" : "Preview image"}
                      className="text-gray-400 hover:text-indigo-600 p-1 rounded hover:bg-gray-50 transition-colors cursor-pointer"
                    >
                      <ZoomIn size={12} />
                    </button>
                  )}
                  {file.url && (
                    <button
                      onClick={(e) => handleDownload(e, file)}
                      title="Download file"
                      className="text-gray-400 hover:text-indigo-600 p-1 rounded hover:bg-gray-50 transition-colors cursor-pointer"
                    >
                      <Download size={12} />
                    </button>
                  )}
                  <button
                    onClick={(e) => {
                      e.stopPropagation();
                      setDeleteConfirmId(file.id);
                    }}
                    title="Delete file"
                    className="text-gray-400 hover:text-rose-600 p-1 rounded hover:bg-gray-50 transition-colors cursor-pointer"
                  >
                    <Trash2 size={12} />
                  </button>
                </div>
              )}
              <span className={`text-[9px] font-bold uppercase px-1.5 py-0.5 rounded font-sans tracking-wide group-hover:hidden ${getFileBadge(detectFileType(file.name))}`}>
                {getFileLabel(detectFileType(file.name))}
              </span>
            </div>
          </div>
        ))}
      </div>

      {previewFile && (
        <div
          className="fixed inset-0 z-50 bg-black/60 flex items-center justify-center p-4 animate-fadeIn"
          onClick={closePreview}
        >
          <div
            className={`relative rounded-xl overflow-hidden shadow-2xl ${
              isPdf(previewFile.name)
                ? 'w-[min(860px,90vw)] h-[85vh]'
                : 'max-w-3xl max-h-[80vh]'
            }`}
            onClick={e => e.stopPropagation()}
          >
            <button
              onClick={closePreview}
              className="absolute top-2 right-2 w-7 h-7 rounded-full bg-black/50 hover:bg-black/70 text-white flex items-center justify-center transition-colors cursor-pointer z-10"
            >
              <X size={14} />
            </button>
            {previewLoading && (
              <div className="w-48 h-48 flex items-center justify-center bg-black/30">
                <Loader2 size={24} className="text-white animate-spin" />
              </div>
            )}
            {previewError && (
              <div className="w-64 h-40 flex flex-col items-center justify-center gap-2 bg-black/40 text-white px-4">
                <AlertCircle size={28} className="text-rose-400" />
                <p className="text-xs text-center text-white/80">File not found on server.<br/>Try deleting and re-uploading it.</p>
              </div>
            )}
            {previewBlobUrl && isPdf(previewFile.name) && (
              <iframe
                src={previewBlobUrl}
                title={previewFile.name}
                className="w-full h-full border-0 block"
              />
            )}
            {previewBlobUrl && !isPdf(previewFile.name) && (
              <img
                src={previewBlobUrl}
                alt={previewFile.name}
                className="block max-w-full max-h-[80vh] object-contain"
              />
            )}
            <div className="absolute bottom-0 left-0 right-0 bg-black/40 px-3 py-1.5 text-white text-[11px] truncate">
              {previewFile.name}
            </div>
          </div>
        </div>
      )}

      {deleteConfirmId !== null && (
        <div 
          className="absolute inset-0 bg-white/95 backdrop-blur-2xs rounded-xl p-4 flex flex-col items-center justify-center text-center z-40 animate-fadeIn"
          onClick={() => setDeleteConfirmId(null)}
        >
          <div className="w-9 h-9 rounded-full bg-rose-50 flex items-center justify-center mb-2">
            <Trash2 size={16} className="text-rose-500" />
          </div>
          <h4 className="text-xs font-bold text-gray-900 mb-1">Delete this file?</h4>
          <p className="text-[10px] text-gray-400 max-w-[200px] mb-4">
            This action cannot be undone.
          </p>
          <div className="flex gap-2 w-full max-w-[180px] text-[10px]">
            <button 
              onClick={() => setDeleteConfirmId(null)}
              className="flex-1 py-1.5 border border-gray-200 rounded-md text-gray-500 bg-white hover:bg-gray-50 transition-colors cursor-pointer"
            >
              Cancel
            </button>
            <button 
              onClick={() => {
                if (deleteConfirmId !== null) {
                  onDelete(deleteConfirmId);
                  setDeleteConfirmId(null);
                }
              }}
              className="flex-1 py-1.5 bg-rose-500 hover:bg-rose-600 text-white font-medium rounded-md transition-colors cursor-pointer"
            >
              Delete
            </button>
          </div>
        </div>
      )}
    </section>
  );
}