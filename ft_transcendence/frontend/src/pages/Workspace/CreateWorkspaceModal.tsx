import React, { useState } from 'react';
import { Plus, UserPlus, Check, X, Loader2, CalendarDays } from 'lucide-react';
import { lookupUser, UserProfile } from '../../api';

interface Props {
  onClose: () => void;
  onCreate: (name: string, description: string, deadline: string | null, inviteLogin?: string) => Promise<void>;
  existingNames: string[];
}

export default function CreateWorkspaceModal({ onClose, onCreate, existingNames }: Props) {
  const [name, setName]         = useState('');
  const [desc, setDesc]         = useState('');
  const [deadline, setDeadline] = useState('');
  const [inviteInput, setInviteInput] = useState('');
  const [error, setError]       = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const [lookupStatus, setLookupStatus] = useState<'idle' | 'loading' | 'found' | 'not_found'>('idle');
  const [foundUser, setFoundUser] = useState<UserProfile | null>(null);

  const isDuplicate = name.trim() !== '' && existingNames.some(n => n.toLowerCase() === name.trim().toLowerCase());

  const handleLookup = async () => {
    const query = inviteInput.trim();
    if (!query) return;
    setLookupStatus('loading');
    setFoundUser(null);
    const user = await lookupUser(query);
    if (user) { setFoundUser(user); setLookupStatus('found'); }
    else       { setLookupStatus('not_found'); }
  };

  const handleInviteChange = (val: string) => {
    setInviteInput(val);
    setLookupStatus('idle');
    setFoundUser(null);
  };

  const handleSubmit = async () => {
    if (!name.trim() || isSubmitting || isDuplicate) return;
    setError('');
    try {
      setIsSubmitting(true);
      const inviteLogin = lookupStatus === 'found' && foundUser ? foundUser.login : undefined;
      await onCreate(name.trim(), desc.trim(), deadline || null, inviteLogin);
      onClose();
    } catch (err: any) {
      setError(err.message || 'Failed to create workspace.');
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="fixed inset-0 bg-black/30 z-50 flex items-center justify-center" onClick={onClose}>
      <div className="bg-white rounded-xl border border-gray-100 shadow-xl w-96 p-6" onClick={e => e.stopPropagation()}>
        <h3 className="text-sm font-bold text-gray-900 mb-1 flex items-center gap-2">
          <Plus size={15} /> New Workspace
        </h3>
        <p className="text-[11px] text-gray-400 mb-4">Fill in the details to set up your new workspace.</p>

        <div className="space-y-3 text-[11px]">
          {/* Name */}
          <div>
            <label className="text-gray-500 font-medium block mb-1">
              Workspace Name <span className="text-red-400">*</span>
            </label>
            <input
              autoFocus
              type="text"
              placeholder="e.g. Design System, Backend API..."
              value={name}
              onChange={e => { setName(e.target.value); setError(''); }}
              className={`w-full border rounded-lg p-2.5 outline-none text-[12px] transition-all ${
                isDuplicate ? 'border-red-300 focus:border-red-400' : 'border-gray-200 focus:border-indigo-500'
              }`}
            />
            {isDuplicate && <p className="text-[10px] text-red-500 mt-1">This name is already taken.</p>}
          </div>

          {/* Description */}
          <div>
            <label className="text-gray-500 font-medium block mb-1">Description</label>
            <textarea
              rows={2}
              placeholder="What is this workspace for?"
              value={desc}
              onChange={e => setDesc(e.target.value)}
              className="w-full border border-gray-200 rounded-lg p-2.5 outline-none focus:border-indigo-500 text-[12px] resize-none transition-all"
            />
          </div>

          {/* Deadline */}
          <div>
            <label className="text-gray-500 font-medium block mb-1 flex items-center gap-1.5">
              <CalendarDays size={11} /> Deadline <span className="text-gray-400 font-normal">(optional)</span>
            </label>
            <input
              type="date"
              value={deadline}
              onChange={e => setDeadline(e.target.value)}
              min={new Date().toISOString().slice(0, 10)}
              className="w-full border border-gray-200 rounded-lg p-2.5 outline-none focus:border-indigo-500 text-[12px] transition-all text-gray-700"
            />
          </div>

          {/* Invite */}
          <div>
            <label className="text-gray-500 font-medium block mb-1 flex items-center gap-1.5">
              <UserPlus size={11} /> Invite Member <span className="text-gray-400 font-normal">(optional)</span>
            </label>
            <div className="flex gap-2">
              <input
                type="text"
                placeholder="Login or username..."
                value={inviteInput}
                onChange={e => handleInviteChange(e.target.value)}
                onKeyDown={e => e.key === 'Enter' && handleLookup()}
                className={`flex-1 border rounded-lg p-2.5 outline-none text-[12px] transition-all ${
                  lookupStatus === 'not_found' ? 'border-red-300 focus:border-red-400' :
                  lookupStatus === 'found'     ? 'border-green-300 focus:border-green-400' :
                  'border-gray-200 focus:border-indigo-500'
                }`}
              />
              <button
                type="button"
                onClick={handleLookup}
                disabled={!inviteInput.trim() || lookupStatus === 'loading'}
                className="px-3 py-2 text-[11px] font-medium bg-gray-100 hover:bg-gray-200 disabled:opacity-40 rounded-lg transition-colors"
              >
                {lookupStatus === 'loading' ? <Loader2 size={12} className="animate-spin" /> : 'Find'}
              </button>
            </div>
            {lookupStatus === 'found' && foundUser && (
              <div className="mt-1.5 flex items-center gap-2 bg-green-50 border border-green-100 rounded-lg px-2.5 py-1.5">
                <Check size={11} className="text-green-500 flex-shrink-0" />
                <span className="text-[11px] text-green-700 font-medium">{foundUser.displayName || foundUser.login}</span>
                <span className="text-[10px] text-green-500">@{foundUser.login}</span>
              </div>
            )}
            {lookupStatus === 'not_found' && (
              <div className="mt-1.5 flex items-center gap-2 bg-red-50 border border-red-100 rounded-lg px-2.5 py-1.5">
                <X size={11} className="text-red-400 flex-shrink-0" />
                <span className="text-[11px] text-red-500">User not found.</span>
              </div>
            )}
          </div>

          {error && (
            <p className="text-[11px] text-red-500 bg-red-50 border border-red-100 rounded-lg px-3 py-2">{error}</p>
          )}
        </div>

        <div className="flex gap-2 mt-5 text-[11px]">
          <button onClick={onClose} className="flex-1 py-2 border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors">
            Cancel
          </button>
          <button
            onClick={handleSubmit}
            disabled={!name.trim() || isDuplicate || isSubmitting}
            className="flex-1 py-2 bg-gray-900 hover:bg-gray-800 disabled:opacity-40 disabled:cursor-not-allowed text-white font-medium rounded-lg transition-colors"
          >
            {isSubmitting ? 'Creating...' : 'Create & Open'}
          </button>
        </div>
      </div>
    </div>
  );
}
