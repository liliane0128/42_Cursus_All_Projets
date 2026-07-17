type Props = {
  target: { id: number; name: string } | null;
  onConfirm: (id: number) => void;
  onCancel: () => void;
};

export default function RemoveFriendDialog({ target, onConfirm, onCancel }: Props) {
  if (!target) return null;
  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black/50">
      <div className="bg-white rounded-xl shadow-xl px-6 py-5 w-[320px] flex flex-col gap-4">
        <p className="text-[13px] font-semibold text-gray-800">
          Remove {target.name} from friends?
        </p>
        <div className="flex flex-col gap-2">
          <button
            onClick={() => onConfirm(target.id)}
            className="w-full py-2 rounded-lg border border-red-200 text-red-500 text-[12px] font-medium hover:bg-red-50 transition-colors"
          >
            Remove
          </button>
          <button
            onClick={onCancel}
            className="w-full py-2 rounded-lg border border-gray-200 text-gray-500 text-[12px] font-medium hover:bg-gray-50 transition-colors"
          >
            Cancel
          </button>
        </div>
      </div>
    </div>
  );
}
